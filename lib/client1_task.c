#include "user_main.h"
#include "client1_task.h"

extern xQueueHandle client1_queue;



/* <|DNS callback for conn1 if there is no maping ip in cache|> */
void ResolveDNS_for_conn1( const char *name, ip_addr_t *ipaddr, void *arg ){
	if(push_mode == 1) {
		os_printf("Can't find ip in cache system. Asking from DNS server\n"); 		 
	}

	if(ipaddr == NULL){
		switch(push_mode){
			case 0: os_printf("Error: %d \n",NO_LOOKUP_IP_FOUND); break;
			case 1: os_printf("Unable to address from %s",name); break;
		}
	}else{
		memcpy(conn1.proto.tcp->remote_ip, ipaddr, 4);
		if(push_mode ==1) {
			os_printf("Connecting . . .\n");
		}
		switch(conn1.state){
			case ESPCONN_CONNECT: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_NONE: espconn_connect(&conn1); break;
			case ESPCONN_LISTEN: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_WAIT: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_WRITE: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_READ: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_CLOSE: espconn_connect(&conn1);  break;
		}
	}
}	


void client1_task(void *pvParameters) {
	cli1_index =0;
		while(1){
			if(xQueueReceive( client1_queue,&buf1, 100000) == pdPASS){
				if(*buf1 == '\r'){			
					os_printf("RAW data inside task: %s\n",cli1_buffer);	
					cli1_index=0;
					char* cli1_argv[10];
					tokenize(cli1_buffer, cli1_argv, MAXARG, cmdstopper, argdelimiter);
					char *comm = command();
					//~ os_printf("arg0 = %s\n",comm);
					//~ os_printf("arg1 = %s\n",argString(1,cli1_argv));
					//~ os_printf("arg2 = %d\n",argInt(2,cli1_argv));
					//~ os_printf("arg3 = %s\n",argString(3,cli1_argv));
					
					if(strcmp(comm,CONNECT_TO_SERVER1_BY_CLIENT1)==0) connect1(argString(1,cli1_argv),argInt(2,cli1_argv));
					if(strcmp(comm,DISCONNECT_FROM_SERVER1)==0)	disconn1();
					if(strcmp(comm,CHECKSTATUS_CLIENT1)==0)	status1();	
					if(strcmp(comm,READ_DATA_FROM_CLIENT1_BUFFER)==0)	read1(argInt(1,cli1_argv));	
					if(strcmp(comm,PRINT_TO_SERVER1)==0)	print1(argString(1,cli1_argv));	
					if(strcmp(comm,READ_DATA_FROM_CLIENT1_BUFFER_TO_ARDUINO_LIB)==0)	read_arduino1(argInt(1,cli1_argv));		
					
				}else{
					cli1_buffer[cli1_index] = *(char*)buf1;
					cli1_buffer[++cli1_index] = '\0';
				}
			}
		}
	vTaskDelete( NULL );	
}

void connect1(char* ip, int port){
	os_printf("Port is : %d\n",port);
	os_printf("IP: %s\n",ip);
	conn1.proto.tcp->remote_port = port;
	if(espconn_gethostbyname(&conn1, ip, &HostResolve_Ip1, ResolveDNS_for_conn1) == ESPCONN_OK){
		memcpy(conn1.proto.tcp->remote_ip, &HostResolve_Ip1, 4);
		switch(conn1.state){
			case ESPCONN_CONNECT: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_NONE: os_printf("Connecting\n"); espconn_connect(&conn1); break;
			case ESPCONN_LISTEN: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_WAIT: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_WRITE: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_READ: os_printf("Error: %d \n", CONN_NOT_READY_TO_CONNECT); break;
			case ESPCONN_CLOSE: os_printf("Connecting\n");  espconn_connect(&conn1);   break;
		}
	}
}

void disconn1(){
	//os_printf("inside discon");
	espconn_disconnect(&conn1);
}

void status1(){
	if(echo_mode==1) os_printf("%s ",CHECKSTATUS_CLIENT1	);
	
	switch(conn1.state){
		case ESPCONN_CONNECT: os_printf("OK\n"); break;
		case ESPCONN_NONE: os_printf("NONE\n"); break;
		case ESPCONN_LISTEN: os_printf("LISTENING\n"); break;
		case ESPCONN_WAIT: os_printf("WAITING\n"); break;
		case ESPCONN_WRITE: os_printf("WRITING\n"); break;
		case ESPCONN_READ: os_printf("READING\n"); break;
		case ESPCONN_CLOSE: os_printf("NOT CONNECTED\n"); break;
	}
}

void read1(int sending_bytes){
	int x;
	char c;
	int buffer_ele = client1_buf->numElements(client1_buf);
	if(buffer_ele != 0){
		if(buffer_ele > sending_bytes){
			for(x = 0;x<sending_bytes;x++){   					
				client1_buf->pull(client1_buf,&c);
				os_printf("%c",c);
			}
		}else {
			for(x = 0;x<buffer_ele;x++){   					
				client1_buf->pull(client1_buf,&c);
				os_printf("%c",c);
			}
		}
	}
}
	
	
void print1(char* payload){
		int index=0;
		while((int)payload[index] != 0){
			//~ os_printf("Payload : %d\n",(int)payload[index]);
			index++;
		}
		//~ os_printf("Index: %d",index);
		if(!(espconn_send(&conn1,payload,index))){
			os_printf("OK\n");				
		}else{
			os_printf("ERROR\n");
		}
}

void read_arduino1(int sending_bytes){
	int x;
	char c;
	int buffer_ele = client1_buf->numElements(client1_buf);
	if(buffer_ele != 0){
		if(buffer_ele > sending_bytes){
			 os_printf("%c",32| sending_bytes);
			for(x = 0;x<sending_bytes;x++){   					
					client1_buf->pull(client1_buf,&c);
					os_printf("%c",c);
			}
		}else {
			os_printf("%c",32| buffer_ele);				
			for(x = 0;x<buffer_ele;x++){   																
					client1_buf->pull(client1_buf,&c);
					os_printf("%c",c);
			}
		}
	}	
}


/* <|DATA RECEIVED CALLBACK (FOR CLIENT 1)|> */ 
void recv_cb1(void *arg, char *pData, unsigned short len){
	int i;
	if(push_mode == 1) {
		os_printf("Received data from server 1");
	}
	for(i=0;i<len;i++)client1_buf->add(client1_buf,(pData+i));
}

void connectCB1(void *arg) {
	os_printf("OK\n");
}

void errorCB1(void *arg, sint8 err) {
	os_printf("Error: %d\n", err);
}

/*	<|CLIENT1 DISCONNECT CALLBACK|> */
void disconnectCB1(void *arg) {
	if(push_mode == 1){
		 os_printf("CLIENT1 DISCONN \n");
	 }
}


