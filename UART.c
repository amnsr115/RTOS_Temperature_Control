
#include "inc\hw_ssi.h"
#include "inc\hw_types.h"
#include "inc\tm4c123gh6pm.h"
#include "inc\hw_memmap.h"
#include "inc\hw_uart.h"
#include "driverlib\pin_map.h"
#include "driverlib\uart.h"
#include "driverlib\ssi.h"
#include "driverlib\gpio.h"
#include "driverlib\sysctl.h"
#include "driverlib\debug.h"

#define BAUDRATE 9600
void UART_init(void)
{// Enabling clock to UART 0 and Port A
		SysCtlClockSet(SYSCTL_SYSDIV_16 | SYSCTL_USE_PLL| SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    
    // Configure GPIO Pins for UART mode.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);
    
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), BAUDRATE,(UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE |UART_CONFIG_WLEN_8));//SysCtlClockGet() -> 16000000 //parity none, one stop bit, 8 bits
}
char UART_receive(void)
{ 
    //waiting until data is received
    while ((UART0_FR_R&(1<<4)) != 0);
    //return the data register content
    return UART0_DR_R;
  }

void UART_transmit(char x)
{
	//Waiting until data can be transmitted
    while ((UART0_FR_R&(1<<5)) != 0);
    //Put the data in the data register
    UART0_DR_R = x;
}
void UART_receive_string (uint8_t* str)
{
  uint8_t k=0;
  str[k] = UART_receive();
  while (str[k] != '.'){
    k++;
    str[k] = UART_receive();
  }
  str[k] = '\0';
}

void UART_transmit_string (uint8_t* str)
	{
  while (*str != '\0'){
    UART_transmit(*str);
    str++;
  }
}
bool UART_Data_Ready(void){
		return ((UART0_FR_R&(1<<4)) == 0);
}