#ifndef MINI_UART_H
#define MINI_UART_H
#include "type.h"
void uart_init(void);
char uart_recv(void);
void uart_send(char);
void uart_send_string(char*);
void uart_hex(uint32_t );
#endif