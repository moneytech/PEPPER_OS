#include "kernel/initialisation/gdt.h"
#include "kernel/initialisation/idt.h"
#include "kernel/initialisation/io.h"
#include "kernel/memory_management/paging/paging.h"
#include "stdlib/video.h"

void main() {
    pepper_screen();
    cli;
    init_paging();
    kprintf(4, ADVICE_COLOR, "Map % at physical address:% \n", 0xFFFFF000, get_phyaddr((void *)0xFFFFF000));

    init_gdt();

    kprintf(2, READY_COLOR, "Chargement des segments\n");

    init_idt();
    sti;

    while (1)
        ;
}