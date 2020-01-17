#include "kernel/initialisation/gdt.h"
#include "kernel/initialisation/idt.h"
#include "kernel/initialisation/io.h"
#include "kernel/memory_management/paging/paging.h"
#include "stdlib/video.h"

void main() {
    pepper_screen();
    cli;
    init_paging();

    physaddr_t *test_address;

    test_address = get_phyaddr((void *)0x3ff000);
    kprintf(3, ADVICE_COLOR, "Map  at physical address:% \n", test_address);

    init_gdt();

    kprintf(2, READY_COLOR, "Chargement des segments\n");

    init_idt();
    sti;

    while (1)
        ;
}