ENTRY(_start)

SECTIONS
{   
    . = 0x80000;
    .text.boot : {KEEP*(.text.boot)}
    .text : {*(.text)}
    .data : {*(.data)}
    .rodata : {*(.rodata)}
    . = ALIGN(0x8);
    bss_begin = . ;
    .bss : {*(.bss*)}
    bss_end = . ;
}