#include "user_main.h"
#include "microgear_task.h"
extern xQueueHandle microgear_queue;



void onConnected(char *attribute, uint8_t* msg, uint16_t msglen) {
    os_printf("OK\n"); 
}

//~ /*	<|NETPIE ON MESSAGE HANDLER CALLBACK|> */
void onMsghandler(char *topic, uint8_t* msg, uint16_t msglen) {
	memset(&str,0,sizeof(struct MgStruct));
	memcpy(str.msg,msg,msglen);
	strcpy(str.topic,topic);

	//~ os_printf("\n\nMsg is: %s\n",str.msg);
	//~ os_printf("Topic is: %s\n",str.topic);
	str.msglen = msglen;
	mg_buf->add(mg_buf,&str);

   if(push_mode == 1){
			int buffer_ele = mg_buf->numElements(mg_buf);
		if(buffer_ele != 0){
			mg_buf->pull(mg_buf,&str_pull);
			os_printf("%s:%s\r\n",str_pull.topic,str_pull.msg);
		}
	}
}


void microgear_task(void *pvParameters) {
	mg_index =0;
		while(1){
			if(xQueueReceive( microgear_queue,&buf3, 100000) == pdPASS){
				if(*buf3 == '\r'){			
					//~ os_printf("RAW data inside task: %s\n",mg_buffer);	
					mg_index=0;
					char* mg_argv[10];
					tokenize(mg_buffer, mg_argv, MAXARG, cmdstopper, argdelimiter);
					char *comm = command();
					//~ os_printf("\narg0 = %s\n",comm);
					//~ os_printf("arg1 = %s\n",argString(1,mg_argv));
					//~ os_printf("arg2 = %s\n",argString(2,mg_argv));
					//~ os_printf("arg3 = %s\n",argString(3,mg_argv));
					//~ os_printf("arg4 = %s\n",argString(4,mg_argv));
					//~ os_printf("arg5 = %s\n",argString(5,mg_argv));
					//~ os_printf("arg6 = %s\n\n",argString(6,mg_argv));
					
					if(strcmp(comm,CONNECT_TO_NETPIE)==0) mg_connect();
					if(strcmp(comm,DISCONNECT_FROM_NETPIE)==0)	mg_disconn();	
					//~ if(strcmp(comm,SETUP_TOKEN)==0) setup_token((argString(1,mg_argv)),(argString(2,mg_argv)));
					if(strcmp(comm,CONFIG_MICROGEAR)==0) config_netpie(argString(1,mg_argv), argString(2,mg_argv), argString(3,mg_argv), argString(4,mg_argv),argString(5,mg_argv),argString(6,mg_argv));	
					if(strcmp(comm,SET_ALIAS_NAME)==0)	set_alias(argString(1,mg_argv));		
					if(strcmp(comm,PUBLISH)==0)	mg_publish(argString(1,mg_argv), argString(2,mg_argv));	
					if(strcmp(comm,SUBSCRIBE)==0)	mg_subscribe(argString(1,mg_argv));		
					if(strcmp(comm,UNSUBSCRIBE)==0) mg_unscribe(argString(1,mg_argv));		
					if(strcmp(comm,CHAT)==0) mg_chat(argString(1,mg_argv), argString(2,mg_argv));
					if(strcmp(comm,PULL_MESSAGE)==0)	mg_pull_buf(argString(1,mg_argv));	
					if(strcmp(comm,WRITE_FEED)==0)mg_writefeed(argInt(1,mg_argv), argString(2,mg_argv), argString(3,mg_argv), argString(4,mg_argv));
					
				}else{
					mg_buffer[mg_index] = *(char*)buf3;
					mg_buffer[++mg_index] = '\0';
				}
			}
		}
	vTaskDelete( NULL );	
}



void mg_connect(){
	if(echo_mode==1) os_printf("%s\n",CONNECT_TO_NETPIE);
	microgear_on(&mg, CONNECTED, onConnected);
	microgear_on(&mg, MESSAGE, onMsghandler);
	microgear_connect(&mg,appid);
	
}

//~ void setup_token(char* token,char* token_secret){
	
	//~ microgear_setToken(&mg, TOKEN, TOKENSECRET, NULL);
//~ }

void config_netpie(char* appid_fn, char*key, char*secret, char* alias, char* token,char* token_secret){
	if(echo_mode==1) os_printf("%s=\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",CONFIG_MICROGEAR,appid_fn,key,secret,alias,token,token_secret);
	strcpy(appid,appid_fn);
	microgear_setup(&mg, token, token_secret, NULL, key, secret, alias);
}


void mg_disconn(){
	if(echo_mode==1) os_printf("%s\n",DISCONNECT_FROM_NETPIE);
	microgear_disconnect(&mg);	
}

void set_alias(char* alias){
	if(echo_mode==1) os_printf("%s=\"%s\"\n",SET_ALIAS_NAME);
	microgear_setAlias(&mg, alias);	
}

void mg_publish(char* topic, char* payload){
	if(echo_mode==1) os_printf("%s=\"%s\",\"%s\"\n",PUBLISH,topic,payload);
	microgear_publish(&mg, topic, payload, NULL);
}

void mg_subscribe(char* topic){
	if(echo_mode==1) os_printf("%s=\"%s\"\n",SUBSCRIBE,topic);
	microgear_subscribe(&mg, topic);
}

void mg_unscribe(char* topic){
	if(echo_mode==1) os_printf("%s=\"%s\"\n",UNSUBSCRIBE,topic);
	microgear_unsubscribe(&mg, topic);		
}

void mg_writefeed(int feedmode, char* feedname, char* feeddata, char* apikey){
	if(echo_mode==1) os_printf("%s=\"%s\",\"%s\",\"%s\",\"%s\"\n",WRITE_FEED,feedmode,feedname,feeddata,apikey);
		feed_buff[0] = '\0';
		strcpy(feed_buff,"/@writefeed/");
		strcat(feed_buff,feedname);
		if(feedmode){
			 strcat(feed_buff,"/"); strcat(feed_buff,apikey);
		 }
		microgear_publish(&mg, feed_buff, feeddata, NULL);
}

void mg_chat(char* device_name, char* payload){
	if(echo_mode==1) os_printf("%s=\"%s\",\"%s\"\n",CHAT,device_name,payload);
	microgear_chat(&mg, device_name, payload);
}


void mg_pull_buf(){
	if(echo_mode==1) os_printf("%s\n",PULL_MESSAGE);
	int buffer_ele = mg_buf->numElements(mg_buf);
	if(buffer_ele != 0){
		mg_buf->pull(mg_buf,&str_pull);
		os_printf("%s:%s\r\n",str_pull.topic,str_pull.msg);
	}
}


