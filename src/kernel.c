#include "mini_uart.h"
#include "mailbox.h"
#include "shell.h"
#include "cpio.h"
#include "type.h"
#include "simple_allocator.h"

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

    // 測試簡單分配器
    uart_send_string("\r\n=== Simple Allocator Test ===\r\n");
    char* test_str = (char*)simple_malloc(8);
    if (test_str) {
        uart_send_string("Allocation successful!\r\n");
        // 寫入一些測試數據
        test_str[0] = 'T';
        test_str[1] = 'e';
        test_str[2] = 's';
        test_str[3] = 't';
        test_str[4] = '\0';
        uart_send_string("Test string: ");
        uart_send_string(test_str);
        uart_send_string("\r\n");
    } else {
        uart_send_string("Allocation failed!\r\n");
    }

    // 測試 CPIO 功能
    uart_send_string("\r\n=== CPIO Test ===\r\n");
    uart_send_string("Listing files in CPIO archive:\r\n");
    cpio_list_files();

    // 測試讀取檔案
    size_t filesize;
    char* content = cpio_get_file("hello.txt", &filesize);
    if (content) {
        uart_send_string("\r\nContent of hello.txt:\r\n");
        for (size_t i = 0; i < filesize; i++) {
            uart_send(content[i]);
        }
        uart_send_string("\r\n");
    } else {
        uart_send_string("\r\nFailed to read hello.txt\r\n");
    }

    shell();
}