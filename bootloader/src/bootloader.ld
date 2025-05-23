SECTIONS
{
    . = 0x60000;
    .text : {
        *(.text.boot)    
        *(.text)         
    }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : { *(.bss) }
}