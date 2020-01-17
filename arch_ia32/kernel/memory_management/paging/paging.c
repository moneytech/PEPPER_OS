#include "paging.h"
#include "../../../stdlib/video.h"

uint32_t page_directory[PAGE_DIRECTORY_OFFSET] __attribute__((aligned(PAGE_DIRECTORY_SIZE)));

uint32_t first_page_table[PAGE_TABLE_OFFSET] __attribute__((aligned(PAGE_TABLE_SIZE)));

void init_paging() {
    uint16_t i = 0;

    for (i = 1; i < PAGE_DIRECTORY_OFFSET; i++) page_directory[i] = 0x2;

    for (i = 0; i < PAGE_TABLE_OFFSET; i++) first_page_table[i] = (i << 12) | 0x3;

    page_directory[0] = ((uint32_t)first_page_table) | 0x3;

    _EnablingPaging_();

    return;
}

physaddr_t *get_phyaddr(void *virtualaddr) {
    uint32_t pdindex = (uint32_t)virtualaddr >> 22;
    uint32_t ptindex = (uint32_t)virtualaddr >> 12 & 0x03FF;

    uint32_t *pd, *pt, ptable;

    if ((page_directory[pdindex] & 0x3) == 0x3) {
        pd = (uint32_t *)(page_directory[pdindex] & 0xFFFFF000);

        if ((pd[ptindex] & 0x3) == 0x3) {
            ptable = pd[ptindex] & 0xFFFFF000;

            pt = (uint32_t *)ptable;

            return (physaddr_t *)(pt + ((uint32_t)(virtualaddr)&0xFFF));
        } else
            return 0x0;
    } else
        return 0x0;
}