ENTRY(_start)

SECTIONS
{
    . = 0x80000;
    __relocate_start = .;
    .relocate : {
        KEEP(*(.text.boot))
        KEEP(*(.text.main))
        *(.rodata*)
        *(.data*)
    }
    __relocate_end = .;

    . = ALIGN(8);
    bss_begin = .;
    .bss : { *(.bss*) }
    bss_end = .;
}
