#ifndef _PAGING_H_
#define _PAGING_H_

#define PAGE_DIRECTORY_OFFSET 0x400
#define PAGE_DIRECTORY_SIZE 0X1000

#define PAGE_TABLE_OFFSET 0X400
#define PAGE_TABLE_SIZE 0X1000

typedef struct phy_addr {
    unsigned int addr_phy;
} physaddr_t;

typedef struct virt_addr {
    unsigned int addr_virt;
} virtaddr_t;

#define PAGE_PRESENT(x) (x)                //Page present in table or directory
#define PAGE_READ_WRITE (1 << 1)           //Page read or write in table or directory
#define PAGE_READ_ONLY (0 << 1)            //Page read only
#define PAGE_USER_SUPERVISOR (1 << 2)      //Access for all
#define PAGE_SUPERVISOR (0 << 2)           //Acces only by supervisor
#define PAGE_WRITE_THROUGH (1 << 3)        //Write through the page
#define PAGE_WRITE_BACK (0 << 3)           //Write back the page
#define PAGE_CACHE_DISABLED(x) ((x) << 4)  //Page will not be cached
#define PAGE_ACCESSED(x) ((x) << 5)        //Page accessed
#define PAGE_SIZE_4KiB (0 << 6)            //Page for 4Kib
#define PAGE_SIZE_4MiB (1 << 6)            //Page for 4Mib
#define PAGE_DIRTY(x) ((x) << 6)           //  Page with dirty flag
#define PAGE_GLOBAL(x) ((x) << 7)

#include "../../../stdlib/i386types.h"

//Initialiation de la pagination
void init_paging();

//Déterminer l'addresse physique à partir d'un addresse virtuelle
physaddr_t *get_phyaddr(void *address_virtuel);

//Activer la pagination
extern void _EnablingPaging_();

// //ELle permet de modifier les options d'une page
void map_page(void *virtual_address, void *physical_address, unsigned int flag);

#define PAGING_LIMIT 0x400000

#endif