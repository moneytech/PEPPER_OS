#include "kernel/initialisation/gdt.h"
#include "kernel/initialisation/idt.h"
#include "kernel/initialisation/io.h"
#include "stdlib/video.h"

void main() {
    pepper_screen();
    init_gdt();
    init_idt();
}
