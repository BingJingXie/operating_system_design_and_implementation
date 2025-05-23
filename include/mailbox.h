#ifndef MAILBOX_H
#define MAILBOX_H
#include "type.h"
// Peripheral base for Pi 3/3B+
#define PERIPH_BASE      0x3F000000
#define MAILBOX_BASE     (PERIPH_BASE + 0x0000B880)
#define MAILBOX_READ     (MAILBOX_BASE + 0x00)
#define MAILBOX_STATUS   (MAILBOX_BASE + 0x18)
#define MAILBOX_WRITE    (MAILBOX_BASE + 0x20)
#define MAILBOX_EMPTY    0x40000000
#define MAILBOX_FULL     0x80000000

// Mailbox property tags
#define REQUEST_CODE        0x00000000
#define END_TAG             0x00000000
#define GET_BOARD_REVISION  0x00010002
#define GET_MEMORY          0x00010005
#define REQUEST_SUCCEED     0x80000000
#define TAG_REQUEST_CODE    0x00000000

int mbox_call(uint8_t,uint32_t []);
uint32_t get_board_revision(void);
void get_memory_base_address_and_size();
#endif