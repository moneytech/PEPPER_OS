#include "paging.h"

uint32_t page_directory[PAGE_DIRECTORY_OFFSET] __attribute__((aligned(PAGE_DIRECTORY_SIZE)));

uint32_t first_page_table[PAGE_TABLE_OFFSET] __attribute__((aligned(PAGE_TABLE_SIZE)));


extern uint32_t error_code;
extern void _FlushPagingCache_();

void init_paging() {
    uint16_t i = 0;

    for (i = 1; i < PAGE_DIRECTORY_OFFSET; i++) page_directory[i] = (PAGE_PRESENT(0) | PAGE_READ_WRITE | PAGE_ACCESSED(0) | PAGE_SUPERVISOR);

    for (i = 0; i < PAGE_TABLE_OFFSET; i++) first_page_table[i] = (i << 12) | (PAGE_PRESENT(1) | PAGE_READ_WRITE | PAGE_ACCESSED(1) | PAGE_SUPERVISOR);

    page_directory[0] = ((uint32_t)first_page_table) | (PAGE_ACCESSED(1) | PAGE_READ_WRITE | PAGE_PRESENT(1) | PAGE_SUPERVISOR);

   _EnablingPaging_();


    return;
}

static inline physaddr_t *_get_page_ (virtaddr_t *virtualaddr)
{
    uint32_t pdindex = (uint32_t)virtualaddr >> 22;
    uint32_t ptindex = (uint32_t)virtualaddr >> 12 & 0x03FF;


//Pour déterminer l'adresse physique , il me sufit de me placer dans la bonne page table
//pour ensuite déteminer les 20 deniers bits et utiliser le décalage des 12 premiers bits
    if ((page_directory[pdindex] & PAGE_VALID) == PAGE_VALID) {
        uint32_t *pd = (uint32_t *)(page_directory[pdindex] & 0xFFFFF000);

        if ((pd[ptindex] & PAGE_VALID) == PAGE_VALID) {
            uint32_t ptable = pd[ptindex] & 0xFFFFF000;

            return (physaddr_t *)(ptable);
        } else
            return (physaddr_t *)("_NoAddress_");
    } else
        return (physaddr_t *)("_NoAddress_");
}


physaddr_t *get_phyaddr(virtaddr_t *virtualaddr) {
	
	uint32_t pdindex = (uint32_t)virtualaddr >> 22;
    uint32_t ptindex = (uint32_t)virtualaddr >> 12 & 0x03FF;

    /*
        A 4-KByte naturally aligned page directory is located at the physical 
         address specified in bits 31:12 of CR3
    */

   if ((page_directory[pdindex] & PAGE_VALID) == PAGE_VALID)
   {
      uint32_t *pt=(uint32_t*)(page_directory[pdindex] & 0xFFFFF000) ;

      if((pt[ptindex] & PAGE_VALID) == PAGE_VALID)
      
        return (physaddr_t*)((pt[ptindex]&0xFFFFF000)+(((uint32_t)virtualaddr)&0xFFF));
      
      else return (physaddr_t*)("_NoPhyAddr_") ;
   }
   else return (physaddr_t*)("_NoPhyAddr_") ;

}

physaddr_t *map_page(virtaddr_t *virtual_address, uint16_t flag_directory, uint16_t flag_table) {
    uint32_t pdindex = (uint32_t)virtual_address >> 22;
    uint32_t ptindex = (uint32_t)virtual_address >> 12 & 0x03FF;

    if ((page_directory[pdindex] & PAGE_VALID) != PAGE_VALID) {  //Le directory n'existe pas

        page_directory[pdindex] = (pdindex << 12) | ((uint32_t)flag_directory & 0xFFF);  //On charge la table directory

        uint32_t *pt = (uint32_t *)(page_directory[pdindex] & 0xFFFFF000);

        pt[ptindex] = (ptindex << 12) | ((uint32_t)flag_table & 0xFFF);  //on charge la page table

        if (DetectPAT) _FlushPagingCache_();

        return (physaddr_t *)((pt[ptindex] & 0xFFFFF000) + ((uint32_t)virtual_address & 0xFFF));  //On modifie l'@ phy

    }

    else  //Le directory existe
    {
        uint32_t *pd = (uint32_t *)(page_directory[pdindex] & 0xFFFFF000);

        if ((pd[ptindex] & PAGE_VALID) == PAGE_VALID)
            return (physaddr_t *)get_phyaddr((void *)virtual_address);  //Le table et le directory existent

        else  //Le directory existe mais le table n'existe pas
        {
            pd[ptindex] = (ptindex << 12) | ((uint32_t)flag_table & 0xFFF);

            if (DetectPAT) _FlushPagingCache_();

            return (physaddr_t *)((pd[ptindex] & 0xFFFFF000) + ((uint32_t)virtual_address & 0xFFF));
        }
    }
}

void unmap_page(virtaddr_t *virtual_address) {
    uint32_t pdindex = (uint32_t)virtual_address >> 22;
    uint32_t ptindex = (uint32_t)virtual_address >> 12 & 0x03FF;

    uint32_t *pd = (uint32_t *)(page_directory[pdindex] & 0xFFFFF000);
    pd[ptindex] = (PAGE_ACCESSED(1) | PAGE_PRESENT(1) | PAGE_READ_WRITE | PAGE_SUPERVISOR);

    _FlushPagingCache_();
}

void InitRangesMemory() {
}

void Paging_fault() {}
