#include "paging.h"
#include "../../../stdlib/video.h"

unsigned int __PAGE_DIRECTORY__[0x400] __attribute__((aligned(0x1000)));

void init_paging() {
    unsigned int i = 0;

    unsigned int __FIRST_PAGE_TABLE__[0x400] __attribute__((aligned(0x1000)));

    for (i = 0; i < 0x400; i++) __PAGE_DIRECTORY__[i] = PAGE_PRESENT(0) | PAGE_READ_WRITE;

    for (i = 0; i < 0x400; i++) __FIRST_PAGE_TABLE__[i] = (i * 0x1000) | PAGE_PRESENT(1) | PAGE_READ_WRITE;

    __PAGE_DIRECTORY__[0] = (unsigned int)__FIRST_PAGE_TABLE__ | PAGE_PRESENT(1) | PAGE_READ_WRITE;

    _EnablingPaging_();
}

void *get_phyaddr(void *vir_addr) {
    unsigned int pdindex = (unsigned int)(vir_addr);
    unsigned int ptindex = (unsigned int)(vir_addr);

    pdindex >>= 22;
    ptindex >>= 12 & 0x03FF;

    //Vérifier si le PD est présent
    if ((__PAGE_DIRECTORY__[pdindex] & 0x03) == (PAGE_PRESENT(1) | PAGE_READ_WRITE)) {
        unsigned int *pt;

        *pt = __PAGE_DIRECTORY__[pdindex] >> 12;  //Take the address of the page table

        pt = (unsigned int *)(*pt);

        if ((pt[ptindex] & 0x03) == (PAGE_PRESENT(1) | PAGE_READ_WRITE)) {
            return (void *)(pt[ptindex] >> 12) + ((unsigned int)(vir_addr)&0x00000FFF);
        } else {
            kprintf(4, ERROR_COLOR, "No page table for %", vir_addr);
            return vir_addr;
        }

    }

    else {
        kprintf(3, ERROR_COLOR, "No page directory for %", vir_addr);
        return vir_addr;
    }
}

void map_page(void *virtual_address, void *physical_address, unsigned int flag) {
    //Verifions les correspondanse
    if (physical_address == (get_phyaddr(virtual_address))) {
        unsigned int pdindex = (unsigned int)(virtual_address);
        unsigned int ptindex = (unsigned int)(virtual_address);

        pdindex >>= 22;
        ptindex >>= 12 & 0x03FF;

        //Vérifier si le PD est présent

        unsigned int *pt;

        *pt = __PAGE_DIRECTORY__[pdindex] >> 12;

        pt = (unsigned int *)(*pt);
    }
}