ENTRY(_start)

SECTIONS
{
    . = 0x80000;
    __relocate_start = .;

    /* 要搬走的可執行／可讀資料 */
    .text : {
        KEEP(*(.text.boot))
        KEEP(*(.text.main))
        *(.text)
        *(.rodata*)
        *(.data*)
    }

    /* 直接接著 .bss，確保連續 */
    . = ALIGN(8);
    bss_begin = .;
    .bss : { *(.bss*) }
    bss_end = .;

    /* end label 放到最後 */
    __relocate_end = .;
    .eh_frame : { *(.eh_frame) }
}
