#include "kernel/initialisation/gdt.h"
#include "kernel/initialisation/idt.h"
#include "kernel/initialisation/io.h"
#include "stdlib/video.h"

void main() {
    pepper_screen();
    cli;
    init_gdt();
    init_idt();
    sti;
    while (1)

        ;
}