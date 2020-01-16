#include "paging.h"
#include "../../../stdlib/video.h"

unsigned int __PAGE_DIRECTORY__[PAGE_DIRECTORY_OFFSET] __attribute__((aligned(PAGE_DIRECTORY_SIZE)));

unsigned int __FIRST_PAGE_TABLE__[PAGE_TABLE_OFFSET] __attribute__((aligned(PAGE_TABLE_SIZE)));

void init_paging() {
    unsigned int i = 0;

    for (i = 0; i < 0x400; i++) __PAGE_DIRECTORY__[i] = PAGE_PRESENT(0) | PAGE_READ_WRITE;

    for (i = 0; i < 0x400; i++) __FIRST_PAGE_TABLE__[i] = (i << 12) | PAGE_PRESENT(1) | PAGE_READ_WRITE;

    __PAGE_DIRECTORY__[0] = ((unsigned int)__FIRST_PAGE_TABLE__) | PAGE_PRESENT(1) | PAGE_READ_WRITE;

    _EnablingPaging_();
}

void *get_phyaddr(void *virtualaddr) {
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

    unsigned long *pd = (unsigned long *)__PAGE_DIRECTORY__[pdindex];

    unsigned long *pt = (unsigned long *)pd[ptindex];

    return (void *)(pt + ((unsigned int)virtualaddr & 0xFFF));
}