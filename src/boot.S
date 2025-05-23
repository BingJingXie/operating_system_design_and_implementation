#include "mm.h"
.section ".text.boot"

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

    mov sp , #LOW_MEMORY
    bl kernel_main
    b endless_loop


