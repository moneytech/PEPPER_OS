#include "kernel/initialisation/gdt.h"
#include "kernel/initialisation/idt.h"
#include "kernel/initialisation/io.h"
#include "kernel/memory_management/paging/paging.h"
#include "stdlib/video.h"

void main() {
    pepper_screen();
    cli;
    init_paging();

    kprintf(4, READY_COLOR, "Map kernel at physical address [%, % ]\n\nInitialisation de 4Mo de memoire physique\n\n", get_phyaddr((void *)0x000), get_phyaddr((void *)0x3ff000));

    init_gdt();

    kprintf(2, READY_COLOR, "Chargement des segments\n\n");

    init_idt();
    sti;

    while (1)
        ;
}