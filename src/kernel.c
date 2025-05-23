#include "mini_uart.h"
#include "mailbox.h"
#include "shell.h"

void kernel_main(void){
    uart_init();
    uint32_t rev = get_board_revision();

    if (rev) {
        uart_send_string("Board Revision: 0x");
        uart_hex(rev);
        uart_send_string("\r\n");
    } else {
        uart_send_string("Failed to get board revision\r\n");
    }
    get_memory_base_address_and_size();
    shell();
}