#include "user_main.h"

	uint32_t current_time = 0;
    uint32_t past_time = 0;
  
	
//~ xQueueHandle wifi_queue,client1_queue,client2_queue,microgear_queue,setup_queue,write_queue;
/***********************************
 * 	
 * 			R E A D  S E R I A L   T a s k
 * 
 * **********************************/	
void read_sr(void *pvParameters) {
	while(1){
		current_time = system_get_time();
		//~ if(current_time - past_time > 2000000){
				//~ //os_printf("reset message index \n");
				//~ past_time = current_time;
				//~ message_index = 0;
		//~ }
		
		if(xQueueReceive(xQueueUART,(void *)&xQueueHandleUart,0) == pdPASS){
			message_sr[message_index++] = xQueueHandleUart.param;
			message_sr[message_index] = '\0';
			
			
			if(xQueueHandleUart.param == 13){
				past_time = system_get_time();
				os_printf("Raw Data: %s\n", message_sr);
				int i;
				
				//~ //Use parser to extract function name.
				int argcount = parse(message_sr);
				char *comm = command();
				 //~ printf("command = %s\n",comm);

				/*******Compare function name with prepared functions and send to specific tasks
				 * 
				 *******************************************************************************/ 
		
				if(strcmp(comm,ECHOMODE)==0 ||strcmp(comm,PUSHMODE)==0  ){
					for(i=0; i< message_index; i ++){	
						xQueueSend( setup_queue,&message_sr[i] ,0);
					}
				}
				
				if(strcmp(comm,SETUPWIFI)==0 ||strcmp(comm,CHECKSTATUS_WIFI)==0  ){
					for(i=0; i< message_index; i ++){	
						xQueueSend( wifi_queue,&message_sr[i] ,0);
					}
				}
			
				
				if(strcmp(comm,CONNECT_TO_SERVER1_BY_CLIENT1)==0 || strcmp(comm,DISCONNECT_FROM_SERVER1)==0 ||strcmp(comm,CHECKSTATUS_CLIENT1)==0 ||strcmp(comm,READ_DATA_FROM_CLIENT1_BUFFER)==0||strcmp(comm,PRINT_TO_SERVER1)==0 ||strcmp(comm,READ_DATA_FROM_CLIENT1_BUFFER_TO_ARDUINO_LIB)==0  ){
					for(i=0; i< message_index; i ++){	
						xQueueSend( client1_queue,&message_sr[i] ,0);
					}
				}
				
				if(strcmp(comm,CONNECT_TO_SERVER2_BY_CLIENT2)==0 || strcmp(comm,DISCONNECT_FROM_SERVER2)==0 ||strcmp(comm,CHECKSTATUS_CLIENT2)==0 ||strcmp(comm,READ_DATA_FROM_CLIENT2_BUFFER)==0||strcmp(comm,PRINT_TO_SERVER2)==0 ||strcmp(comm,READ_DATA_FROM_CLIENT2_BUFFER_TO_ARDUINO_LIB)==0  ){
					for(i=0; i< message_index; i ++){	
						xQueueSend( client2_queue,&message_sr[i] ,0);
					}
				}
				
				if(strcmp(message_sr,"microgear task\r")==0){
					os_printf("Inside microgear loop");
					for(i=0; i< message_index; i ++){	
						xQueueSend( microgear_queue,&message_sr[i] ,0);
					}
				}
				
				if(strcmp(message_sr,"write task\r")==0){
					os_printf("Inside write loop");
					for(i=0; i< message_index; i ++){	
						xQueueSend( write_queue,&message_sr[i] ,0);
					}
				}
				message_index = 0; 
			}
		}
		vTaskDelay(10 / portTICK_RATE_MS);
	}
	vTaskDelete( NULL );
}

/***********************************
 * 	
 * 			W R I T E   S E R I A L  T a s k
 * 
 * **********************************/	

  
/***********************************
 * 	
 * 			S E T U P  T a s k
 * 			Param: mode
 * 
 * **********************************
void setup_task(void *pvParameters) {
	while(1){
	char setup_buf[128],buf[1];
		int index=0;;
		while(1){
			if(xQueueReceive( setup_queue,&buf,2000 ) == pdPASS){
				setup_buf[index] = *(char*)buf;
				setup_buf[++index] = '\0';
				
				if(*(char*)buf == '\r'){
					//Use parser to extract variable and function name (Function name, var1, var2, var3, var4, var 5)
								
					//Compare function name 		
					os_printf("Queue data: %s\n",setup_buf);
					if(strcmp(setup_buf,"setup task\r") == 0){
						os_printf("Inside setup task");
						index = 0;
						setup_buf[index] = '\0';
					}
				
				}
				
			}
		}
	}
	vTaskDelete( NULL );
}
*/


/***********************************
 * 	
 * 			C L I E N T  1   T a s k
 * 			Param: IP,port,payload,size_to_read
 * 
 * **********************************
 
void client1_task(void *pvParameters) {
	while(1){
		char client1_buf[128],buf[1];
		int index=0;;
		while(1){
			if(xQueueReceive( client1_queue,&buf,2000 ) != pdPASS){
				index=0;
				client1_buf[index] = '\0';
			}else{
				client1_buf[index] = *(char*)buf;
				client1_buf[++index] = '\0';
			}
		}
	}
	vTaskDelete( NULL );
}

 */
 
/***********************************
 * 	
 * 			C L I E N T  2  T a s k
 * 			Param: IP,port,payload,size_to_read
 * 
 * *********************************
 void client2_task(void *pvParameters) {
	while(1){
		char client2_buf[128],buf[1];
		int index=0;;
		while(1){
			if(xQueueReceive( client2_queue,&buf,2000 ) != pdPASS){
				index=0;
				client2_buf[index] = '\0';
			}else{
				client2_buf[index] = *(char*)buf;
				client2_buf[++index] = '\0';
			
			}
		}	
	}
	vTaskDelete( NULL );
}
* */
 
/***********************************
 * 	
 * 			M I C R O G E A R   T a s k
 * 			Param: token,token_secret,key,secret,alias,appid,topic,payload,mode,feedname,apikey
 * 
  * **********************************	
  
  void microgear_task(void *pvParameters) {
	while(1){
	char mg_buf[128],buf[1];
		int index=0;;
		while(1){
			if(xQueueReceive( microgear_queue,&buf,2000 ) != pdPASS){
				index=0;
				mg_buf[index] = '\0';
			}else{
				mg_buf[index] = *(char*)buf;
				mg_buf[++index] = '\0';
			}
		}
	}
	vTaskDelete( NULL );
}
 
 */


/**********************************************************************************************************************
*											 USER INIT PART           																																					  *
***********************************************************************************************************************/
void ICACHE_FLASH_ATTR user_init(void) {
	
	InitParser();
	push_mode=1;
	echo_mode=1;
	//setupwifi("catbus","stud2559");
	
   
    //~ //////////* Uart init *//////////
    uart_param_t para;
	para.uart_baud_rate = UART_BAUD_RATE_9600;
	para.uart_xfer_bit = UART_XFER_8_BIT;
	para.uart_parity_mode = UART_PARITY_NONE;
	para.uart_stop_bit = UART_1_STOP_BIT;
	para.uart_flow_ctrl = UART_NONE_FLOW_CTRL;
	uart0_init(&para);
	

	
	
	
	//////////* Semaphore init *//////////
	vSemaphoreCreateBinary(WifiReady);
	xSemaphoreTake(WifiReady, 0);              // take the default semaphore
	vSemaphoreCreateBinary(SetWifi);
	xSemaphoreTake(SetWifi, 0);
	microgear_setWifiSemaphore(&WifiReady);
	
	
		//////////* tcp1 init *//////////
	espconn_init();								//HAVE TO BE CALL UNLESS THE CONN WILL FAIL
	conn1.type = ESPCONN_TCP;
	conn1.state = ESPCONN_NONE;
	conn1.proto.tcp = &tcp1;
	conn1.proto.tcp->local_port=espconn_port();
	conn2.type = ESPCONN_TCP;
	conn2.state = ESPCONN_NONE;
	conn2.proto.tcp = &tcp2;
	conn2.proto.tcp->local_port=espconn_port();
	
	
	/////////* Register callback function for espconn1 */////////
	espconn_regist_connectcb(&conn1,connectCB1);
	espconn_regist_reconcb(&conn1, errorCB1);
	espconn_regist_disconcb(&conn1,disconnectCB1);
	espconn_regist_recvcb(&conn1,recv_cb1);
	
	//////////* Queue init *m//////////
	wifi_queue = xQueueCreate( 128,sizeof(char));
	client1_queue = xQueueCreate( 128,sizeof(char)); 
	client2_queue = xQueueCreate( 128,sizeof(char));
	microgear_queue = xQueueCreate( 128,sizeof(char));
	setup_queue = xQueueCreate( 128,sizeof(char));
	write_queue = xQueueCreate( 128,sizeof(char));
	
	
	//////////* Task init *//////////////
	//~ xTaskCreate(task2, "wifi_task", 254,NULL,tskIDLE_PRIORITY + 1, NULL);
	//~ xTaskCreate(task1, "give_wifiready_semaphore", 254,NULL,tskIDLE_PRIORITY + 1, NULL);
	
	xTaskCreate(read_sr, "read_sr", 254,NULL,tskIDLE_PRIORITY + 3, NULL);
	xTaskCreate(setup_task, "setup", 254,NULL,tskIDLE_PRIORITY + 3, NULL);
	xTaskCreate(wifi_task, "wifi", 254,NULL,tskIDLE_PRIORITY + 3, NULL);
	xTaskCreate(give_wifi_semaphore, "give_wifiready_semaphore", 254,NULL,tskIDLE_PRIORITY + 3, NULL);
	xTaskCreate(client1_task, "client1", 254,NULL,tskIDLE_PRIORITY + 3, NULL);
	//~ xTaskCreate(client2_task, "client2", 254,NULL,tskIDLE_PRIORITY + 1, NULL);
	//~ xTaskCreate(microgear_task, "microgear", 254,NULL,tskIDLE_PRIORITY + 1, NULL);
	
	
	//////////* Ring buf init *//////////
	client1_buf = (RingBuf *)RingBuf_new(1, 1024);
	client2_buf = (RingBuf *)RingBuf_new(1, 1024);
	mg_buf = RingBuf_new(sizeof(struct MgStruct ), 20);
	
	
   
}
