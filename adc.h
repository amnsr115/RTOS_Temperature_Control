#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>

/////////////////////// Functions /////////////////////////////////////

void ADC_init(void);                       //Initalize ADC0 SS3
uint32_t ADC_Read(void);                   //Returns the value of the ADC

///////////////////////////////////////////////////////////////////////

#endif 