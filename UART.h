#ifndef UART_MODULE
#define UART_MODULE

#define PART_TM4C123GH6PM

#include <stdint.h>
#include <stdbool.h>

#define BAUDRATE 115200

extern void UART_init(void);
extern char UART_receive(void);
extern void UART_transmit(char x);
extern void UART_transmit_string(uint8_t* data);
extern void UART_receive_string(uint8_t* str);

#endif //UART_MODULE