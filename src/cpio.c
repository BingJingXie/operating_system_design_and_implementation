#include "cpio.h"
#include "mini_uart.h"
#include "shell.h"
#include "type.h"

static inline uint32_t hex2int(const char *str, int len) {
    uint32_t val = 0;
    for (int i = 0; i < len; i++) {
        char c = str[i];
        val <<= 4;
        if (c >= '0' && c <= '9') val |= c - '0';
        else if (c >= 'A' && c <= 'F') val |= c - 'A' + 10;
        else if (c >= 'a' && c <= 'f') val |= c - 'a' + 10;
    }
    return val;
}

void* cpio_get_file(const char* pathname, size_t* filesize) {
    uint8_t* ptr = (uint8_t*)CPIO_ADDR;
    while (1) {
        cpio_newc_header_t* hdr = (cpio_newc_header_t*)ptr;
        // 將魔術數字複製到臨時緩衝區並加上 null 結尾
        char magic[7] = {0};
        for(int i = 0; i < 6; i++) {
            magic[i] = hdr->c_magic[i];
        }
        if (strcmp(magic, "070701") != 0) break;

        uint32_t namesize = hex2int(hdr->c_namesize, 8);
        uint32_t filesize_ = hex2int(hdr->c_filesize, 8);

        char* name = (char*)(ptr + sizeof(cpio_newc_header_t));
        // 4 bytes 對齊
        uint32_t name_pad = ((sizeof(cpio_newc_header_t) + namesize + 3) & ~3);
        uint8_t* file_data = ptr + name_pad;

        if (strcmp(name, "TRAILER!!!") == 0) break;

        if (strcmp(name, pathname) == 0) {
            if (filesize) *filesize = filesize_;
            return file_data;
        }

        // 移到下一個 entry
        uint32_t file_pad = ((filesize_ + 3) & ~3);
        ptr = file_data + file_pad;
    }
    return NULL;
}

void cpio_list_files(void) {
    uint8_t* ptr = (uint8_t*)CPIO_ADDR;
    while (1) {
        cpio_newc_header_t* hdr = (cpio_newc_header_t*)ptr;
        // 將魔術數字複製到臨時緩衝區並加上 null 結尾
        char magic[7] = {0};
        for(int i = 0; i < 6; i++) {
            magic[i] = hdr->c_magic[i];
        }
        if (strcmp(magic, "070701") != 0) break;

        uint32_t namesize = hex2int(hdr->c_namesize, 8);
        uint32_t filesize = hex2int(hdr->c_filesize, 8);

        char* name = (char*)(ptr + sizeof(cpio_newc_header_t));
        // 4 bytes 對齊
        uint32_t name_pad = ((sizeof(cpio_newc_header_t) + namesize + 3) & ~3);
        uint8_t* file_data = ptr + name_pad;

        if (strcmp(name, "TRAILER!!!") == 0) break;

        uart_send_string("File: ");
        uart_send_string(name);
        uart_send_string(" (");
        uart_hex(filesize);
        uart_send_string(" bytes)\r\n");

        // 移到下一個 entry
        uint32_t file_pad = ((filesize + 3) & ~3);
        ptr = file_data + file_pad;
    }
} 