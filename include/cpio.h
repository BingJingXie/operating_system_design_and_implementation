#ifndef _CPIO_H
#define _CPIO_H

#include "type.h"

#define CPIO_ADDR 0x20000000  // RPi3 預設載入位址

typedef struct {
    char c_magic[6];      // 魔術數字，必須是 "070701"
    char c_ino[8];        // inode 編號
    char c_mode[8];       // 檔案模式
    char c_uid[8];        // 使用者 ID
    char c_gid[8];        // 群組 ID
    char c_nlink[8];      // 硬連結數
    char c_mtime[8];      // 修改時間
    char c_filesize[8];   // 檔案大小
    char c_devmajor[8];   // 主要裝置號碼
    char c_devminor[8];   // 次要裝置號碼
    char c_rdevmajor[8];  // 主要裝置號碼（特殊檔案）
    char c_rdevminor[8];  // 次要裝置號碼（特殊檔案）
    char c_namesize[8];   // 檔名長度
    char c_check[8];      // 校驗和
} cpio_newc_header_t;

// 解析 CPIO 檔案並回傳檔案內容
void* cpio_get_file(const char* pathname, size_t* filesize);

// 列出 CPIO 檔案中的所有檔案
void cpio_list_files(void);

#endif 