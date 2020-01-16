#include "paging.h"
#include "../../../stdlib/video.h"

unsigned int page_directory[PAGE_DIRECTORY_OFFSET] __attribute__((aligned(PAGE_DIRECTORY_SIZE)));

unsigned int first_page_table[PAGE_TABLE_OFFSET] __attribute__((aligned(PAGE_TABLE_SIZE)));

void init_paging() {
    unsigned int i = 0;

    for (i = 0; i < PAGE_DIRECTORY_OFFSET; i++) page_directory[i] = PAGE_PRESENT(0) | PAGE_READ_WRITE;

    for (i = 0; i < PAGE_TABLE_OFFSET; i++) first_page_table[i] = (i << 12) | PAGE_PRESENT(1) | PAGE_READ_WRITE;

    page_directory[0] = ((unsigned int)first_page_table) | PAGE_PRESENT(1) | PAGE_READ_WRITE;

    _EnablingPaging_();
}

void *get_phyaddr(void *virtualaddr) {
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

    unsigned long *pd = (unsigned long *)page_directory[pdindex];

    unsigned long *pt = (unsigned long *)pd[ptindex];

    return (void *)(pt + ((unsigned int)virtualaddr & 0xFFF));
}