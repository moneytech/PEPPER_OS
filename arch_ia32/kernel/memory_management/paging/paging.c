#include "paging.h"
#include "../../../stdlib/video.h"

uint32_t page_directory[PAGE_DIRECTORY_OFFSET] __attribute__((aligned(PAGE_DIRECTORY_SIZE)));

uint32_t first_page_table[PAGE_TABLE_OFFSET] __attribute__((aligned(PAGE_TABLE_SIZE)));

void init_paging() {
    uint16_t i = 0;

    for (i = 1; i < PAGE_DIRECTORY_OFFSET; i++) page_directory[i] = (PAGE_PRESENT(0) | PAGE_READ_WRITE | PAGE_ACCESSED(0));

    for (i = 0; i < PAGE_TABLE_OFFSET; i++) first_page_table[i] = (i << 12) | (PAGE_PRESENT(1) | PAGE_READ_WRITE | PAGE_ACCESSED(1));

    page_directory[0] = ((uint32_t)first_page_table) | (PAGE_ACCESSED(1) | PAGE_READ_WRITE | PAGE_PRESENT(1));

    _EnablingPaging_();

    return;
}

physaddr_t *get_phyaddr(void *virtualaddr) {
    uint32_t pdindex = (uint32_t)virtualaddr >> 22;
    uint32_t ptindex = (uint32_t)virtualaddr >> 12 & 0x03FF;

    if ((page_directory[pdindex] & 0x23) == 0x23) {
        uint32_t *pd = (uint32_t *)(page_directory[pdindex] & 0xFFFFF000);

        if ((pd[ptindex] & 0x23) == 0x23) {
            uint32_t ptable = pd[ptindex] & 0xFFFFF000;

            return (physaddr_t *)(ptable + ((uint32_t)(virtualaddr)&0xFFF));
        } else
            return (physaddr_t *)("NoAddress");
    } else
        return (physaddr_t *)("NoAddress");
}

void map_page(void *virtual_address, void *physical_address, uint16_t flag_directory, uint16_t flag_table) {
    uint32_t pdindex = (uint32_t)virtual_address >> 22;
    uint32_t ptindex = (uint32_t)virtual_address >> 12 & 0x03FF;

    if ((page_directory[pdindex] & 0x3) != 0x3) {                                        //Le directory n'existe pas
        page_directory[pdindex] = (pdindex << 12) | ((uint32_t)flag_directory & 0xFFF);  //On charge la table directory

        uint32_t *pt = (uint32_t *)(page_directory[pdindex] & 0xFFFFF000);

        pt[ptindex] = (ptindex << 12) | ((uint32_t)flag_table & 0xFFF);  //on charge la page table

        physical_address = (void *)((pt[ptindex] & 0xFFFFF000) + ((uint32_t)virtual_address & 0xFFF));  //On modifie l'@ phy
    }

    else  //Le directory existe
    {
        uint32_t *pd = (uint32_t *)(page_directory[pdindex] & 0xFFFFF000);

        if ((pd[ptindex] & 0x3) == 0x3)
            physical_address = (void *)get_phyaddr((void *)virtual_address);  //Le table et le directory existent

        else  //Le directory existe mais le table n'existe pas
        {
            pd[ptindex] = (ptindex << 12) | ((uint32_t)flag_table & 0xFFF);

            physical_address = (void *)((pd[ptindex] & 0xFFFFF000) + ((uint32_t)virtual_address & 0xFFF));
        }
    }
}