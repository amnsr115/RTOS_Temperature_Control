/* 
CSE 347 Embedded Systems Design
Abanoub Tarek 17P8196
Abdelrahman Atef Abdelrahman Kotb Mansour 17P8185
Bishoy Sameh 17P8096
Omar Zohdy 17P8190
*/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "TM4C123GH6PM.h"
#include "inc\tm4c123gh6pm.h"
/*
#include "inc\hw_ssi.h"
#include "inc\hw_types.h"
#include "inc\hw_memmap.h"
#include "inc\hw_uart.h"
#include "driverlib\pin_map.h"
#include "driverlib\uart.h"
#include "driverlib\ssi.h"
#include "driverlib\gpio.h"
#include "driverlib\sysctl.h"
#include "driverlib\debug.h"
*/
#include <FreeRTOS.h>
#include "FreeRTOSConfig.h"
#include <task.h>
#include "queue.h"
#include "lcd.h"
#include "adc.h"
//#include "UART.h"*/
/////////////////////////Definitions///////////////////////////////

#define LED_RED   (1U << 1) 
#define LED_BLUE  (1U << 2) 
#define LED_GREEN (1U << 3)
#define RELAY     (1U << 2)  //PortB
#define Buzzer    (1U << 3)  //PortB
typedef struct Message
{
	uint16_t SP;
	uint16_t TP;
	
} AMessage;

/////////////////////////Function Prototypes///////////////////////////////

void GPIO_init(void);
void Main_Task(void *pv_parameters);
void LCD_Task(void *pv_parameters);
void UART_Task(void *pv_parameters);
void Buzzer_Task(void *pv_parameters);

/////////////////////////Queues///////////////////////////////

QueueHandle_t xUARTQueue;
QueueHandle_t xLCDQueue;
QueueHandle_t xBuzzerQueue;

/////////////////////////Function Definitions///////////////////////////////

void GPIO_init(void){
    SYSCTL_RCGCGPIO_R |= 0x00000020;              
    while((SYSCTL_PRGPIO_R&0x00000020) == 0){}
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_DIR_R |= (LED_RED | LED_BLUE | LED_GREEN);                 
    GPIO_PORTF_DEN_R |= (LED_RED | LED_BLUE | LED_GREEN);
		GPIO_PORTB_DIR_R |= (RELAY | Buzzer );                 
    GPIO_PORTB_DEN_R |= (RELAY | Buzzer );
}

void Main_Task(void *pv_parameters){
			AMessage LCD_msg;
			uint16_t setpoint = 30;
			uint16_t Real_temp;
			uint16_t ADC_Value;
	    uint32_t adc;
			float mv;
			uint16_t buzz;
			uint8_t MaxTemp = 50;

	for(;;)
  {	
		//xQueueReceive(xUARTQueue, &setpoint, 0);
		adc = ADC_Read();                 //Read ADC Value
		mv = (2000.0/4096.0)*adc;         //Convert to mV
		Real_temp = mv/10;                //Calculate the Temp
		if(Real_temp < setpoint)          
		{
			GPIO_PORTB_DATA_R |= RELAY;         //Turn on relay if temp not reached setpoint
		}
		else
		{
		GPIO_PORTB_DATA_R &=~ RELAY;         //Turn oFF relay if temp surpassed reached setpoint
		}
		LCD_msg.SP = setpoint;               //Send setpoint and measured temp to LCD task
		LCD_msg.TP = Real_temp;		
		xQueueSend(xLCDQueue,&LCD_msg,0);
	
			if(Real_temp > MaxTemp)
			{
				buzz = 1;
			}
			else
			{
				buzz = 0;
			}
			xQueueSend(xBuzzerQueue,&buzz,0);  //Send Buzz state to LCD Task
			taskYIELD();
  }
}

void LCD_Task(void *pv_parameters){


		LCD_Clear();
		LCD_PrintLn(0, "Measured: ");
		LCD_PrintLn(1, "Setpoint: ");
	for(;;)
  {					AMessage msg;
						xQueueReceive(xLCDQueue, &msg, 0);  //Recieve temp values
						LCD_Cursor(0,11);
            LCD_Show((msg.TP/10)+'0');
						LCD_Show((msg.TP%10)+'0');         //Print the Masured Temp
						LCD_Cursor(1,11);
		        LCD_Show((msg.SP/10)+'0');
						LCD_Show((msg.SP%10)+'0');         //Print the setpoint
						vTaskDelay(1000/portTICK_RATE_MS);
		}		
}
void UART_Task(void *pv_parameters){
int sp = 30;
	for(;;)
  {	
	float temp;
	UART_init();
	char k[100] = "Please input Temperature";
	UART_transmit_string (k[100]);
	sp = UART_receive(); 
	xQueueSend(xLCDQueue,&sp,0);

  }		
}
void Buzzer_Task(void *pv_parameters){

	
	uint16_t buzzstate = 0;
	for(;;)
  {	
		xQueueReceive(xBuzzerQueue, &buzzstate, 0);     // Recieve Buzz state and act acordingly
		if(buzzstate == 1)
		{
			GPIO_PORTB_DATA_R |= Buzzer;
		}
		else
		{
			GPIO_PORTB_DATA_R &=~ Buzzer;
		}
		vTaskDelay(1000/portTICK_RATE_MS);
  }		
}
/////////////////////////Main Program///////////////////////////////
int main()
 {
	LCD_init();
	GPIO_init();
	ADC_init();
//	UART_init();
	 
	xLCDQueue = xQueueCreate( 1, 8 );
	xUARTQueue = xQueueCreate( 1, 8 );
	xBuzzerQueue = xQueueCreate( 1, 1 );
	 
	xTaskCreate(Main_Task,"Task1", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate(LCD_Task,"Task2", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
//	xTaskCreate(UART_Task,"Task3", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate(Buzzer_Task,"Task4", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	 
	vTaskStartScheduler();
	
for(;;);

}