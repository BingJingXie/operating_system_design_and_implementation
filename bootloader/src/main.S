.section .text.main
.global relocated_main
relocated_main:
    // 初始化 UART 並印出歡迎訊息
    bl uart_init
    ldr x0, welcome_msg
    bl uart_send_string

    // 讀取 4-byte kernel 大小（big-endian）
    mov x19, #0
    mov x20, #4
recv_loop:
    bl uart_recv
    lsl x19, x19, #8
    orr x19, x19, x0
    subs x20, x20, #1
    b.ne recv_loop

    // 印出 kernel size
    ldr x0, size_msg
    bl uart_send_string
    mov x0, x19
    bl uart_hex

    // 接收 kernel 到 0x80000（原來 bootloader 位址）
    ldr x0, =0x80000
    mov x1, x19
    bl receive_kernel

    // 印完成訊息
    ldr x0, done_msg
    bl uart_send_string

    // 跳轉執行 kernel
    ldr x0, =0x80000
    br x0

// 接收 kernel 映像檔
// x0: 目標地址, x1: 大小
receive_kernel:
    mov x2, x0
    mov x3, x1
recv_kernel_loop:
    bl uart_recv
    strb w0, [x2], #1
    subs x3, x3, #1
    b.ne recv_kernel_loop
    ret

// 字串資料
welcome_msg:
    .string "Bootloader: Waiting for kernel...\r\n"
size_msg:
    .string "Kernel size: 0x"
done_msg:
    .string "Kernel loaded. Jumping to kernel...\r\n"

