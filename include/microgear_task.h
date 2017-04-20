
//~ extern xQueueHandle microgear_queue;

//~ void microgear_task(void *pvParameters) {
	//~ while(1){
	//~ char mg_buf[128],buf[1];
		//~ int index=0;;
		//~ while(1){
		//~ if(xQueueReceive( microgear_queue,&buf,2000 ) != pdPASS){
			//~ index=0;
			//~ mg_buf[index] = '\0';
		//~ }else{
			//~ mg_buf[index] = *(char*)buf;
			//~ mg_buf[++index] = '\0';
	//~ /*	<|MICROGEAR CONNECT|> */		
				//~ if(strcmp(message_sr,CONNECT_TO_NETPIE)==0){	
					//~ microgear_connect(&mg,appid);
					//~ microgear_on(&mg, CONNECTED, onConnected);
					//~ microgear_on(&mg, MESSAGE, onMsghandler);
					//~ message_index=0;
				//~ }
				
				//~ /*	<|MICROGEAR DISCONNECT|> */
				//~ if(strcmp(message_sr,DISCONNECT_FROM_NETPIE)==0){
					//~ microgear_disconnect(&mg);	
					//~ message_index=0;
				//~ }
					
				 //~ /*	<|MICROGEAR SETUP TOKEN|> */
				//~ if(strcmp(message_sr,SETUP_TOKEN)==0){
					//~ message_index=0;
					//~ tokensecret[0] = '\0';
					//~ token[0] = '\0';
					//~ param=0;
					//~ message_sr[0] = '\0';
					//~ while((xQueueReceive(xQueueUART,(void *)&xQueueHandleUart,0) == pdPASS )&& break_loop == 0){	
						//~ if(xQueueHandleUart.param == 34 || xQueueHandleUart.param == 44){
							//~ param++;
							//~ message_index=0;
							//~ switch (param){
								//~ case 1: break;
								//~ case 2:	strcpy(token,message_sr);break;
								//~ case 3: break;
								//~ case 4: break;
								//~ case 5: 
									//~ strcpy(tokensecret,message_sr);
									//~ if(push_mode == 1){
										 //~ os_printf("Token: %s\nToken Secret: %s\n",token,tokensecret);
									 //~ }
									//~ microgear_setToken(&mg, token, tokensecret, NULL);
									//~ break_loop = 1;
									//~ break;
							//~ }
						//~ }else{
							//~ message_sr[message_index++] = xQueueHandleUart.param;							
							//~ message_sr[message_index]=0;
						//~ }
						//~ vTaskDelay(10/ portTICK_RATE_MS);				
					//~ }
					//~ message_index=0;
				//~ }

				/*	<|MICROGEAR INIT (SETUP APPID,KEY,SECRET,ALIAS)|> */
				//~ if(strcmp(message_sr,INIT_MICROGEAR)==0){
					//~ message_index=0;
					//~ param=0;
					//~ appid[0] = '\0';
					//~ key[0] = '\0';
					//~ secret[0] = '\0';
					//~ alias[0] = '\0';
					//~ message_sr[0]= '\0';
					//~ while((xQueueReceive(xQueueUART,(void *)&xQueueHandleUart,0) == pdPASS) && break_loop == 0 ){
						//~ if(xQueueHandleUart.param == 34 || xQueueHandleUart.param == 44 ){
							//~ param++;
							//~ message_index=0;
							//~ switch (param){
								//~ case 1: break;
								//~ case 2: strcpy(appid,message_sr);break;
								//~ case 3: break;
								//~ case 4: break;
								//~ case 5: strcpy(key,message_sr);break;
								//~ case 6: break;
								//~ case 7: break;
								//~ case 8: strcpy(secret,message_sr);break;
								//~ case 9: break;
								//~ case 10: break;
								//~ case 11:
									//~ strcpy(alias,message_sr);
									//~ if(push_mode == 1){
										 //~ os_printf("Appid: %s\nKey: %s\nSecret: %s\nAlias: %s\n",appid,key,secret,alias);	
									 //~ }
									//~ microgear_init(&mg, key, secret, alias); 
									//~ break_loop=1;
									//~ break;
							//~ }
						//~ }else{
							//~ message_sr[message_index++] = xQueueHandleUart.param;
							//~ message_sr[message_index]=0;
						//~ }
						//~ vTaskDelay(10/ portTICK_RATE_MS);				
					//~ }
					//~ message_index=0;
				//~ }
				
				//~ /*	<|MICROGEAR SET ALIAS|> */
				//~ if(strcmp(message_sr,SET_ALIAS_NAME)==0){
					//~ message_index=0;
					//~ message_sr[0] = '\0';
					//~ param = 0;
					//~ while((xQueueReceive(xQueueUART,(void *)&xQueueHandleUart,0) == pdPASS) && break_loop == 0){	
						//~ if(xQueueHandleUart.param == 34){ 
							//~ param++;  												
							//~ message_index=0;
							//~ switch (param){
								//~ case 1: break;
								//~ case 2:	
									//~ if(push_mode ==1){
										//~ os_printf("Set alias name to: %s\n",message_sr);
									//~ }
									//~ microgear_setAlias(&mg, message_sr);	
									//~ break_loop =1;
									//~ break;
					//~ }										
						//~ }else{
							//~ message_sr[message_index++] = xQueueHandleUart.param;
							//~ message_sr[message_index]=0;
						//~ }
						//~ vTaskDelay(10/ portTICK_RATE_MS);				
					//~ }
					//~ message_index=0;
				//~ }
				
				//~ /*	<|MICROGEAR PUBLISH|> */
				//~ //should not use white space in data_pub
				//~ if(strcmp(message_sr,PUBLISH)==0){
					//~ message_index=0;
					//~ param =0;
					//~ message_sr[0] = '\0'; 
					//~ topic_pub[0] = '\0';
					//~ data_pub[0] = '\0';
					//~ while((xQueueReceive(xQueueUART,(void *)&xQueueHandleUart,0) == pdPASS) && break_loop == 0){
						//~ if(xQueueHandleUart.param == 44 || xQueueHandleUart.param == 34 ){
							//~ param++;
							//~ message_index=0;
							//~ switch (param){
								//~ case 1: break;
								//~ case 2: strcpy(topic_pub,message_sr);break;
								//~ case 3: break;
								//~ case 4: break;
								//~ case 5:	
									//~ strcpy(data_pub,message_sr);
									//~ if(push_mode ==1) {
										//~ os_printf("Topis is: %s \ndata is: %s\n",topic_pub,message_sr);
									//~ }
									//~ microgear_publish(&mg, topic_pub, data_pub, NULL);
									//~ break_loop =1;
									//~ break;
							//~ }
						//~ }else{
							//~ message_sr[message_index++] = xQueueHandleUart.param;
							//~ message_sr[message_index]=0;
						//~ }
						//~ vTaskDelay(10/ portTICK_RATE_MS);				
					//~ }
					//~ message_index=0;
				//~ }
				
				//~ /*	<|MICROGEAR SUBSCRIBE|> */
				//~ if(strcmp(message_sr,SUBSCRIBE)==0){
					//~ message_index=0;
					//~ message_sr[0] = '\0';
					//~ param=0;
					//~ while((xQueueReceive(xQueueUART,(void *)&xQueueHandleUart,0) == pdPASS) && break_loop == 0){	
						//~ if(xQueueHandleUart.param == 34){
							//~ param++;
							//~ message_index=0;
							//~ switch (param){
								//~ case 1: break;
								//~ case 2: 
									//~ if(push_mode ==1) {
										//~ os_printf("Subscribe topic: %s\n",message_sr);
									//~ }
									//~ microgear_subscribe(&mg, message_sr);
									//~ break_loop=1;
									//~ break;
							//~ }												
						//~ }else{
							//~ message_sr[message_index++] = xQueueHandleUart.param;
							//~ message_sr[message_index]=0;
						//~ }
						//~ vTaskDelay(10/ portTICK_RATE_MS);				
					//~ }
					//~ message_index=0;
				//~ }
				
				//~ /*	<|MICROGEAR UNSUBSCRIBE|> */
				//~ if(strcmp(message_sr,UNSUBSCRIBE)==0){
					//~ message_index=0;
					//~ message_sr[0] = '\0';
					//~ param =0;
					//~ while((xQueueReceive(xQueueUART,(void *)&xQueueHandleUart,0) == pdPASS) && break_loop == 0){	
						//~ if(xQueueHandleUart.param == 34){
							//~ message_index=0;
							//~ param++;
							//~ switch(param) {
								//~ case 1: break;
								//~ case 2: 
									//~ if(push_mode ==1) os_printf("Unsubscribe topic: %s\n",message_sr);
									//~ microgear_unsubscribe(&mg, message_sr);		
									//~ break_loop=1;
									//~ break;
							//~ }										
						//~ }else{
							//~ message_sr[message_index++] = xQueueHandleUart.param;
							//~ message_sr[message_index]=0;
						//~ }
						//~ vTaskDelay(10/ portTICK_RATE_MS);				
					//~ }
				//~ message_index=0;
				//~ }
				
				
				//~ /*	<|MICROGEAR CHAT|> */
				//~ if(strcmp(message_sr,CHAT)==0){
					//~ message_index=0;
					//~ message_sr[0] = '\0';
					//~ data_chat[0] = '\0';
					//~ device_name[0] = '\0';
					//~ param =0;
					//~ while((xQueueReceive(xQueueUART,(void *)&xQueueHandleUart,0) == pdPASS) && break_loop == 0){
						//~ if(xQueueHandleUart.param == 34 || xQueueHandleUart.param == 44){
							//~ param++;
							//~ message_index=0;
							//~ switch (param){
								//~ case 1: break;
								//~ case 2: strcpy(device_name,message_sr);break;
								//~ case 3: break;
								//~ case 4: break;
								//~ case 5: strcpy(data_chat,message_sr); break_loop =1; break;
							//~ }
						//~ }else{
							//~ message_sr[message_index++] = xQueueHandleUart.param;
							//~ message_sr[message_index]=0;
						//~ }
						//~ vTaskDelay(10/ portTICK_RATE_MS);				
					//~ }
					//~ microgear_chat(&mg, device_name, data_chat);
					//~ message_index=0;
				//~ }	
				
				//~ /* 	<| PULL MESSAGE TO SERIAL FROM MICROGEAR BUFFER |>	*/
				//~ if(strcmp(message_sr,PULL_MESSAGE)==0){				
					//~ int buffer_ele = mg_buf->numElements(mg_buf);
					//~ if(buffer_ele != 0){
						//~ mg_buf->pull(mg_buf,&str_pull);
						//~ os_printf("%s:%s\r\n",str_pull.topic,str_pull.msg);
					//~ }			
					//~ message_index=0;				
				//~ }
				
				//~ /* 	<| WRITE DATA TO FEED |>	*/
				//~ if(strcmp(message_sr,WRITE_FEED)==0){
					//~ message_index=0;
					//~ feeddata[0] = '\0';
					//~ feedname[0] = '\0';
					//~ feedmode = 0;
					//~ apikey[0] = '\0';
					//~ feed_buff[0] = '\0';
					//~ strcpy(feed_buff,"/@writefeed/");
					//~ param =0;
					//~ while((xQueueReceive(xQueueUART,(void *)&xQueueHandleUart,0) == pdPASS) && break_loop == 0){
						//~ if(xQueueHandleUart.param == 34 || xQueueHandleUart.param == 44){
							//~ param++;
							//~ message_index=0;
							//~ switch (param){
								//~ case 1: feedmode = atoi(message_sr);break;
								//~ case 2: break;
								//~ case 3: strcpy(feedname,message_sr);break;
								//~ case 4: break;
								//~ case 5: break;
								//~ case 6: strcpy(feeddata,message_sr);break;
								//~ case 7:break;
								//~ case 8:break;
								//~ case 9: 
								//~ if(feedmode) strcpy(apikey,message_sr);
								//~ break_loop=1;
								//~ break;
							//~ }
						//~ }else{
							//~ message_sr[message_index++] = xQueueHandleUart.param;
							//~ message_sr[message_index]=0;
						//~ }	
						//~ vTaskDelay(10/ portTICK_RATE_MS);				
					//~ }
					//~ strcat(feed_buff,feedname);
					//~ if(feedmode){
						 //~ strcat(feed_buff,"/"); strcat(feed_buff,apikey);
					 //~ }
					//~ if(push_mode == 1) {
						//~ os_printf("Topis: %s\n",feed_buff);
						//~ os_printf("Data: %s\n",feeddata);
					//~ }
					//~ microgear_publish(&mg, feed_buff, feeddata, NULL);
					//~ message_index=0;
				//~ }
		//~ }
//~ }

//~ vTaskDelete( NULL );
//~ }
