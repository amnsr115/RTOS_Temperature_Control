#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "functions.h"




void SystickISR(void);
 uint32_t counter=0;
void Blue_LED(void)//Toggle Blue LED once every 1000ms 
{
  for(;;){

  if(counter%20==0)//Turn OFF after 2000ms (10*2 = 20 counts)
  {
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);
    }
    else if(counter%10==0)//Turn ON after 1000 ms (10 counts)
    {
      GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIO_PIN_2);
    }
  }

}
void Red_LED(void)      //Toggle Red LED once every 500ms (5 counts)
{
  for(;;){
  if(counter%10==0)     //Turn OFF after 1000ms (5*2 = 10 counts)
  {
     GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);
  }
  else if(counter%5==0) //Turn ON after 500 ms (5 counts)
  {
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_PIN_1);
  }
}
}
void Project_init(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
  
  //Setup the GPIO pins for the bue and red LED as outputs
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);  // Set Red pin as outpt PortF pin 1
   GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2); // Set Blue pin as outpt PortF pin 2
   
   //Settup the systic timer
   SysTickDisable();                    //Disable the Timer before Setup
    SysTickPeriodSet(1599999);          //Set to count every 100 millisecond
    SysTickIntEnable();                 //Enable the systic interupt
    SysTickIntRegister(&SystickISR);    //Setup the Systic  ISR
    SysTickEnable();                    //Enable after modifications
}

void SystickISR(void){
  counter++;
  }
