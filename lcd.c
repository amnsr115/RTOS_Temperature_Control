#include <stdbool.h>
#include <stdint.h>
#include "lcd.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "string.h"

//////////////////// INIT. OF LCD /////////////////////////
///////////////////   4 BIT MODE  ////////////////////////

void LCD_init() 
{
        SysCtlPeripheralEnable(LCDPORTENABLE);
        GPIOPinTypeGPIOOutput(LCDPORT, 0xFF);

        SysCtlDelay(50000);                                                   // DELAY 

        GPIOPinWrite(LCDPORT, RS,  0x00 );

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );                     // WRITE IN THIS PORT "GPIO_PORTB_BASE"
        GPIOPinWrite(LCDPORT, E, 0x02);                                       // OPEN ENABLE 
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);                                       // CLEAR ENABLE

        SysCtlDelay(50000);                                                   // DELAY 

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
        GPIOPinWrite(LCDPORT, E, 0x02);                                       // OPEN ENABLE 
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);                                       // CLEAR ENABLE

        SysCtlDelay(50000);                                                   // DELAY 

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
        GPIOPinWrite(LCDPORT, E, 0x02);                                       // OPEN ENABLE 
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);                                       // CLEAR ENABLE

        SysCtlDelay(50000);                                                   // DELAY 

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x20 );
        GPIOPinWrite(LCDPORT, E, 0x02);                                       // OPEN ENABLE 
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);                                      // CLEAR ENABLE

        SysCtlDelay(50000);                                                  // DELAY 


        LCD_Command(0x0F);                                                   //Turn on Lcd
        LCD_Clear();                                                         //Clear screen

}

/////////////////////////////////////////////////////////////////////

/////////////////////  LCD COMMAND  (4 BIT MODE) ////////////////////

void LCD_Command(unsigned char c)
{

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xf0) );             // WRITE COMMAND 
        GPIOPinWrite(LCDPORT, RS, 0x00);
        GPIOPinWrite(LCDPORT, E, 0x02);                                    // OPEN ENABLE 

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, E, 0x00);                                    // CLEAR ENABLE

        SysCtlDelay(50000);                                                // DELAY 

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0x0f) << 4 );        // WRITE COMMAND ( 4 BIT MODE ) 
        GPIOPinWrite(LCDPORT, RS, 0x00);
        GPIOPinWrite(LCDPORT, E, 0x02);                                   // OPEN ENABLE 
        
        SysCtlDelay(10);

        GPIOPinWrite(LCDPORT, E, 0x00);                                   // CLEAR ENABLE

        SysCtlDelay(50000);                                               // DELAY 

}

////////////////////////////////////////////////////////////

//////////////////// SHOW CHAR /////////////////////////////

void LCD_Show(unsigned char d)
{

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0xf0) );       // SHOW ONE CHAR. 
        GPIOPinWrite(LCDPORT, RS, 0x01);
        GPIOPinWrite(LCDPORT, E, 0x02);

        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);                              // CLEAR ENABLE 
        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0x0f) << 4 );  // 4 BIT MODE 
        GPIOPinWrite(LCDPORT, RS, 0x01);
        GPIOPinWrite(LCDPORT, E, 0x02);

        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);                             // CLEAR ENABLE
        SysCtlDelay(50000);

}

/////////////////////////////////////////////////////////////


////////////////////// LCD CURSOR ///////////////////////////

void LCD_Cursor(char x, char y)
{

    if (x==0) {
        LCD_Command(0x80 + (y % 16));
        return;
    }
    LCD_Command(0xC0 + (y % 16));

}

/////////////////////////////////////////////////////////////

////////////////////// CLEAR DISPLAY ///////////////////////
void LCD_Clear(void)
{

        LCD_Command(0x01); 
        SysCtlDelay(10);

}
////////////////////////////////////////////////////////////

//////////////////// LCD PRINT TWO LINE  ///////////////////

void LCD_Print(char *s, char *d)
{
    int j;
    for (j=0; j<16; j++) {
        if (j<strlen(s)) {
            LCD_Cursor(0,j);
            LCD_Show(s[j]);
        }
        if (j<strlen(d)) {
            LCD_Cursor(1,j);
            LCD_Show(d[j]);
        }
    }
    LCD_Command(0xC0 + 16); //Hide cursor
}

//////////////////////////////////////////////////////////////

///////////////// PRINT ONE LINE  ////////////////////////////

void LCD_PrintLn(char i, char *s) {
    LCD_Cursor(i, 0);
    for (i=0; i<strlen(s); i++) {
        LCD_Show(s[i]);
    }
    LCD_Command(0xC0 + 16); //Hide cursor
}

//////////////////////////////////////////////////////////////