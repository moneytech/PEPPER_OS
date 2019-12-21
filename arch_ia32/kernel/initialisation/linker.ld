OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
    
    .text : {*(.text)}

    .data : {*(.data)}

    .rodata : {*(.rodata)}

    .bss :
    {
        *(COMMON)
        *(.bss)
    }

    end = .; _end = .; __end = .;
}
