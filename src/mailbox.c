#include "type.h"
#include "utils.h"      // extern get32, put32, delay
#include "mini_uart.h"
#include "mailbox.h"

// Aligned mailbox buffer
volatile uint32_t mbox_buffer_board[7] __attribute__((aligned(16)));

/**
 * Send the property‐tag message in mbox_buffer over 'channel' and wait for reply.
 */
int mbox_call(uint8_t channel , volatile uint32_t mbox_buffer[]) {
    uint32_t addr = ((uint32_t)((uintptr_t)mbox_buffer) & ~0xF) | (channel & 0xF);
    // Wait until mailbox not full
    while (get32(MAILBOX_STATUS) & MAILBOX_FULL) {}
    // Write the address + channel
    put32(MAILBOX_WRITE, addr);
    // Wait for response
    for (;;) {
        while (get32(MAILBOX_STATUS) & MAILBOX_EMPTY) {}
        if (get32(MAILBOX_READ) == addr)
            return (mbox_buffer[1] & REQUEST_SUCCEED) == REQUEST_SUCCEED;
    }
}

/**
 * Query the GPU for the board revision.
 * @return 32‐bit revision code, or 0 on failure.
 */
uint32_t get_board_revision(void) {
    mbox_buffer_board[0] = 7 * 4;              // buffer size in bytes
    mbox_buffer_board[1] = REQUEST_CODE;       // this is a request message
    mbox_buffer_board[2] = GET_BOARD_REVISION; // tag ID
    mbox_buffer_board[3] = 4;                  // value buffer length (bytes)
    mbox_buffer_board[4] = TAG_REQUEST_CODE;   // must be zero on request
    mbox_buffer_board[5] = 0;                  // clear output value
    mbox_buffer_board[6] = END_TAG; // end tag

    if (!mbox_call(8 , mbox_buffer_board))                   // 8 = property tags channel
        return 0;
    return mbox_buffer_board[5];
}
volatile uint32_t mbox_buffer_mem[8] __attribute__((aligned(16)));

void get_memory_base_address_and_size(){
    mbox_buffer_mem[0] = 8 * 4;              // buffer size in bytes
    mbox_buffer_mem[1] = REQUEST_CODE;       // this is a request message
    mbox_buffer_mem[2] = GET_MEMORY;         // tag ID
    mbox_buffer_mem[3] = 8;                  // value buffer length (bytes)
    mbox_buffer_mem[4] = TAG_REQUEST_CODE;   // must be zero on request
    mbox_buffer_mem[5] = 0;                  // clear output value(base address)
    mbox_buffer_mem[6] = 0;                  // clear output value(size)
    mbox_buffer_mem[7] = END_TAG; // end tag 
    if (mbox_call(8 , mbox_buffer_mem)){
        uart_send_string("Memory Base address: 0x");
        uart_hex(mbox_buffer_mem[5]);
        uart_send_string("\r\n");
        uart_send_string("Memory size: 0x");
        uart_hex(mbox_buffer_mem[6]);
        uart_send_string("\r\n");        
    }else                   
        uart_send_string("Failed to get memory size\r\n");
     
}

