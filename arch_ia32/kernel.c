#include "stdlib/video.h"
#include "kernel/initialisation/io.h"
#include "kernel/initialisation/gdt.h"
#include "kernel/initialisation/idt.h"

void main() {
    pepper_screen();
    cli ;
    init_gdt();
    init_idt();
        sti ;
}
