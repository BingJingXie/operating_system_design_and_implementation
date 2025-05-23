#include "utils.h"
.section .text.boot
.global _start

_start:
    // 初始化堆疊指標
    ldr x0, =_start
    mov sp, x0

    // 初始化UART
    bl uart_init

    // 印出歡迎訊息
    ldr x0, =welcome_msg
    bl uart_send_string

    // 等待接收kernel大小（4 bytes）
    mov x19, #0          // 用於儲存kernel大小
    mov x20, #4          // 需要讀取的位元組數
1:
    bl uart_recv
    lsl x19, x19, #8     // 左移8位
    orr x19, x19, x0     // 加入新的位元組
    subs x20, x20, #1    // 減少計數器
    b.ne 1b              // 如果還沒讀完4個位元組，繼續讀取

    // 印出kernel大小
    ldr x0, =size_msg
    bl uart_send_string
    mov x0, x19
    bl uart_hex

    // 接收kernel映像檔
    ldr x0, =0x80000     // kernel載入位置
    mov x1, x19          // kernel大小
    bl receive_kernel

    // 印出完成訊息
    ldr x0, =done_msg
    bl uart_send_string

    // 跳轉到kernel
    ldr x0, =0x80000
    br x0

// 接收kernel映像檔的函數
// x0: 目標地址
// x1: 大小
receive_kernel:
    mov x2, x0           // 目標地址
    mov x3, x1           // 剩餘大小
1:
    bl uart_recv         // 讀取一個位元組
    strb w0, [x2], #1    // 儲存位元組並增加地址
    subs x3, x3, #1      // 減少剩餘大小
    b.ne 1b              // 如果還沒讀完，繼續讀取
    ret

welcome_msg:
    .string "Bootloader: Waiting for kernel...\r\n"
size_msg:
    .string "Kernel size: 0x"
done_msg:
    .string "Kernel loaded. Jumping to kernel...\r\n" 