#include "kernel/initialisation/gdt.h"
#include "kernel/initialisation/idt.h"
#include "kernel/initialisation/io.h"
#include "kernel/memory_management/paging/paging.h"
#include "stdlib/video.h"

void main() {
    pepper_screen();
    cli;
    init_paging();
    unsigned int *address_main = (unsigned int *)get_phyaddr((void *)0x9000);

    kprintf(3, READY_COLOR, "Map kernel at physical address:% \n", address_main);
    init_gdt();

    kprintf(2, READY_COLOR, "Chargement des segments\n");

    init_idt();
    sti;

    while (1)
        ;
}