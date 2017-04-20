#include "user_main.h"
#include "client2_task.h"

extern xQueueHandle client2_queue;



/* <|DNS callback for conn2 if there is no maping ip in cache|> */
void ResolveDNS_for_conn2( const char *name, ip_addr_t *ipaddr, void *arg ){
	if(push_mode == 1) {
		os_printf("Can't find ip in cache system. Asking from DNS server\n"); 		 
	}

	if(ipaddr == NULL){
		switch(push_mode){
			case 0: os_printf("Error: %d \n",NO_LOOKUP_IP_FOUND); break;
			case 1: os_printf("Unable to address from %s",name); break;
		}
	}else{
		memcpy(conn2.proto.tcp->remote_ip, ipaddr, 4);
		if(push_mode ==1) {
			os_printf("Connecting . . .\n");
		}
		switch(conn2.state){
			case ESPCONN_CONNECT: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_NONE: espconn_connect(&conn2); break;
			case ESPCONN_LISTEN: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_WAIT: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_WRITE: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_READ: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_CLOSE: espconn_connect(&conn2);  break;
		}
	}
}	


void client2_task(void *pvParameters) {
	cli2_index =0;
		while(1){
			if(xQueueReceive( client2_queue,&buf2, 100000) == pdPASS){
				if(*buf2 == '\r'){			
					os_printf("RAW data inside task: %s\n",cli2_buffer);	
					cli2_index=0;
					char* cli2_argv[10];
					tokenize(cli2_buffer, cli2_argv, MAXARG, cmdstopper, argdelimiter);
					char *comm = command();
					//~ os_printf("arg0 = %s\n",comm);
					//~ os_printf("arg1 = %s\n",argString(1,cli2_argv));
					//~ os_printf("arg2 = %d\n",argInt(2,cli2_argv));
					//~ os_printf("arg3 = %s\n",argString(3,cli2_argv));
					
					if(strcmp(comm,CONNECT_TO_SERVER2_BY_CLIENT2)==0) connect2(argString(1,cli2_argv),argInt(2,cli2_argv));
					if(strcmp(comm,DISCONNECT_FROM_SERVER2)==0)	disconn2();
					if(strcmp(comm,CHECKSTATUS_CLIENT2)==0)	status2();	
					if(strcmp(comm,READ_DATA_FROM_CLIENT2_BUFFER)==0)	read2(argInt(1,cli2_argv));	
					if(strcmp(comm,PRINT_TO_SERVER2)==0)	print2(argString(1,cli2_argv));	
					if(strcmp(comm,READ_DATA_FROM_CLIENT2_BUFFER_TO_ARDUINO_LIB)==0)	read_arduino2(argInt(1,cli2_argv));		
					
				}else{
					cli2_buffer[cli2_index] = *(char*)buf2;
					cli2_buffer[++cli2_index] = '\0';
				}
			}
		}
	vTaskDelete( NULL );	
}

void connect2(char* ip, int port){
	os_printf("Port is : %d\n",port);
	os_printf("IP: %s\n",ip);
	conn2.proto.tcp->remote_port = port;
	if(espconn_gethostbyname(&conn2, ip, &HostResolve_Ip2, ResolveDNS_for_conn2) == ESPCONN_OK){
		memcpy(conn2.proto.tcp->remote_ip, &HostResolve_Ip2, 4);
		switch(conn2.state){
			case ESPCONN_CONNECT: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_NONE: os_printf("Connecting\n"); espconn_connect(&conn2); break;
			case ESPCONN_LISTEN: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_WAIT: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_WRITE: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_READ: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_CLOSE: os_printf("Connecting\n");  espconn_connect(&conn2);   break;
		}
	}
}

void disconn2(){
	//os_printf("inside discon");
	espconn_disconnect(&conn2);
}

void status2(){
	if(echo_mode==1) os_printf("%s ",CHECKSTATUS_CLIENT2	);
	
	switch(conn2.state){
		case ESPCONN_CONNECT: os_printf("OK\n"); break;
		case ESPCONN_NONE: os_printf("NONE\n"); break;
		case ESPCONN_LISTEN: os_printf("LISTENING\n"); break;
		case ESPCONN_WAIT: os_printf("WAITING\n"); break;
		case ESPCONN_WRITE: os_printf("WRITING\n"); break;
		case ESPCONN_READ: os_printf("READING\n"); break;
		case ESPCONN_CLOSE: os_printf("NOT CONNECTED\n"); break;
	}
}

void read2(int sending_bytes){
	int x;
	char c;
	int buffer_ele = client2_buf->numElements(client2_buf);
	if(buffer_ele != 0){
		if(buffer_ele > sending_bytes){
			for(x = 0;x<sending_bytes;x++){   					
				client2_buf->pull(client2_buf,&c);
				os_printf("%c",c);
			}
		}else {
			for(x = 0;x<buffer_ele;x++){   					
				client2_buf->pull(client2_buf,&c);
				os_printf("%c",c);
			}
		}
	}
}
	
	
void print2(char* payload){
		int index=0;
		while((int)payload[index] != 0){
			//~ os_printf("Payload : %d\n",(int)payload[index]);
			index++;
		}
		//~ os_printf("Index: %d",index);
		if(!(espconn_send(&conn2,payload,index))){
			os_printf("OK\n");				
		}else{
			os_printf("ERROR\n");
		}
}

void read_arduino2(int sending_bytes){
	int x;
	char c;
	int buffer_ele = client2_buf->numElements(client2_buf);
	if(buffer_ele != 0){
		if(buffer_ele > sending_bytes){
			 os_printf("%c",32| sending_bytes);
			for(x = 0;x<sending_bytes;x++){   					
					client2_buf->pull(client2_buf,&c);
					os_printf("%c",c);
			}
		}else {
			os_printf("%c",32| buffer_ele);				
			for(x = 0;x<buffer_ele;x++){   																
					client2_buf->pull(client2_buf,&c);
					os_printf("%c",c);
			}
		}
	}	
}


/* <|DATA RECEIVED CALLBACK (FOR CLIENT 2)|> */ 
void recv_cb2(void *arg, char *pData, unsigned short len){
	int i;
	if(push_mode == 1) {
		os_printf("Received data from server 2");
	}
	for(i=0;i<len;i++)client2_buf->add(client2_buf,(pData+i));
}

void connectCB2(void *arg) {
	os_printf("OK\n");
}

void errorCB2(void *arg, sint8 err) {
	os_printf("Error: %d\n", err);
}

/*	<|CLIENT2 DISCONNECT CALLBACK|> */
void disconnectCB2(void *arg) {
	if(push_mode == 1){
		 os_printf("CLIENT2 DISCONN \n");
	 }
}


