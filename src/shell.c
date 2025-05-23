#include "mini_uart.h"
#include "peripherals/reboot.h"
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

void shell()
{
    char buffer[128];
    int index;

    uart_send_string("Welcome to RPi3 Shell\r\n");
    uart_send_string("Type 'help' to see available commands.\r\n");

    while (1) {
        uart_send_string("# ");

        index = 0;
        char c;
        while (1) {
            c = uart_recv();

            if (c == '\r') {
                uart_send('\r');  // carriage return
                uart_send('\n');  // line feed
                buffer[index] = '\0';
                break;
            } else if (c == '\b' || c == 127) { // handle backspace
                if (index > 0) {
                    index--;
                    uart_send('\b');
                    uart_send(' ');
                    uart_send('\b');
                }
            } else {
                if (index < sizeof(buffer) - 1) {
                    buffer[index++] = c;
                    uart_send(c); // echo
                }
            }
        }

        // Handle command
        if (index == 0) continue;

        if (strcmp(buffer, "help") == 0) {
            uart_send_string("help  : Print this help menu\r\n");
            uart_send_string("hello : Print Hello World!\r\n");
            uart_send_string("reboot: reboot the device\r\n");
        } else if (strcmp(buffer, "hello") == 0) {
            uart_send_string("Hello World!\r\n");
        } else if(strcmp(buffer, "reboot") == 0){
            reboot();           
        }else {
            uart_send_string("Unknown command: ");
            uart_send_string(buffer);
            uart_send_string("\r\n");
        }
    }
}