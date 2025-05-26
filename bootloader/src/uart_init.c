#include "utils.h"
#include "peripherals/gpio.h"
#include "peripherals/mini_uart.h"
#include "type.h"
char uart_recv(void){
    while(1){
        if(get32(AUX_MU_LSR_REG) &0x1)
            break;
    }
    return get32(AUX_MU_IO_REG) & 0xFF;
}

void uart_send(char c){
    while(1){
        if(get32(AUX_MU_LSR_REG) &0x20)
            break;        
    }
    put32(AUX_MU_IO_REG , c);
}

void uart_send_string(char *str){
    for(int i = 0 ; str[i]!= '\0' ; i++){
        uart_send(str[i]);
    }
}

static const char hex_chars[] = "0123456789ABCDEF";
void uart_hex(uint32_t value) {
    for (int shift = 28; shift >= 0; shift -= 4) {
        uint32_t nibble = (value >> shift) & 0xF;
        uart_send(hex_chars[nibble]);
    }
}

void uart_init(void){
    unsigned int selector;
    selector = get32(GPFSEL1);
    selector &= ~(7 << 12);
    selector |= 2 << 12;
    selector &= ~(7 << 15);
    selector |= 2 << 15;
    put32(GPFSEL1 , selector);

    put32(GPPUD , 0);
    delay(150);
    put32(GPPUDCLK0 , 3 << 14);
    delay(150);
    put32(GPPUDCLK0 , 0);

    put32(AUX_ENABLES , 1);
    put32(AUX_MU_CNTL_REG , 0);
    put32(AUX_MU_IER_REG , 0);
    put32(AUX_MU_LCR_REG,3);
    put32(AUX_MU_MCR_REG,0);
    put32(AUX_MU_BAUD_REG,270);
    put32(AUX_MU_IIR_REG , 6);
    put32(AUX_MU_CNTL_REG , 3);

}