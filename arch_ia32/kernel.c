#include "stdlib/video.h"
#include "kernel/initialisation/io.h"
#include "kernel/initialisation/gdt.h"
#include "kernel/initialisation/idt.h"

void main() {
    pepper_screen();
    cli ;
    init_gdt();
    init_idt();

    __asm__ __volatile__("movw $0x18, %ax \n \
        movw %ax, %ss \n \
        movl $0x20000, %esp");
 sti ;
}
