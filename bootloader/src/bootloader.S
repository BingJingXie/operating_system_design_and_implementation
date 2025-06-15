.section .text.boot
.global _start
_start:
    mrs x0 , mpidr_el1
    and x0 , x0 , #0xff
    cbz x0 , master
    b endless_loop

endless_loop:
    b endless_loop

master:
    adr x0 , bss_begin
    adr x1 , bss_end
    sub x1 , x1 , x0
    bl memzero

    ldr x0, =0x50000
    mov sp, x0

    // 搬移 bootloader 自己到 0x60000
    ldr x0, =__relocate_start
    ldr x1, =__relocate_end
    ldr x2, =0x60000
    mov x3, x2
    sub x4, x1, x0        // size
    mov  x5, #0x20000
    cmp  x4, x5
    b.hs relocate_too_big


copy_loop:
    cmp x0, x1
    b.ge copy_done
    ldrb w4, [x0], #1
    strb w4, [x3], #1
    b copy_loop

copy_done:
    // 計算 relocated_main 的偏移量並跳轉
    ldr x0, =relocated_main     // 取得 relocated_main 的絕對位址
    ldr x3, =__relocate_start   // 原始程式碼起始位址
    sub x0, x0, x3              // 計算 relocated_main 相對於起始位址的偏移量
    add x0, x2, x0              // 計算 relocated_main 在新位置的位址
    br x0                       // 跳轉至 relocated_main

relocate_too_big:
    ldr x0, oversize_msg
    bl  uart_send_string
    b   endless_loop

oversize_msg:
    .string "Bootloader too large!\r\n"
