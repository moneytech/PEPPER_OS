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

/*
    *   ELle permet de modifier les options d'une page
    *   Retourne 'NoAddress' si il n'est pas paginé
*/
physaddr_t *get_phyaddr(virtaddr_t *virtualaddr);

//Activer la pagination
extern void _EnablingPaging_();

// //ELle permet de modifier les options d'une page
physaddr_t *map_page(virtaddr_t *virtual_address, uint16_t flag_directory, uint16_t flag_table);

#include "../../../stdlib/lib.h"
//Determiner la technologie de Pagination
//Detecté le PSE
/*
    PSE: page-size extensions for 32-bit paging.
    If CPUID.01H:EDX.PSE [bit 3] = 1, CR4.PSE may be set to 1, enabling support for 4-MByte pages with 32-bit
    paging
*/
#define DetectPSE32bit ((cpuid(0x1) & 0x08) >> 0x3)

//Detecté le PGE
/*
    PGE: global-page support.
    If CPUID.01H:EDX.PGE [bit 13] = 1, CR4.PGE may be set to 1, enabling the global-page feature 
*/
#define DetectPGE ((cpuid(0x1) & 0x2000) >> 13)

//Detecté le PAT
/*
    PAT: page-attribute table.
    If CPUID.01H:EDX.PAT [bit 16] = 1, the 8-entry page-attribute table (PAT) is supported. When the PAT is
    supported, three bits in certain paging-structure entries select a memory type (used to determine type of
    caching used) from the PAT (see Section 4.9.2).
*/
#define DetectPAT ((cpuid(0x1) & 0x10000) >> 16)

//Detecté le support d'addresse linéaire
/*
   CPUID.80000008H:EAX[15:8] reports the linear-address width supported by the processor. Generally, this
    value is 48 if CPUID.80000001H:EDX.LM [bit 29] = 1 and 32 otherwise. (Processors that do not support CPUID
    function 80000008H, support a linear-address width of 32.)
*/
#define LinearAddress ((cpuid(0x80000001) & 0x20000000) >> 29)

//Detecté le support d'adressage physique
/*
    CPUID.80000008H:EAX[7:0] reports the physical-address width supported by the processor. (For processors
    that do not support CPUID function 80000008H, the width is generally 36 if CPUID.01H:EDX.PAE [bit 6] = 1
    and 32 otherwise.) This width is referred to as MAXPHYADDR. MAXPHYADDR is at most 52.
*/
#define PhysicalAddress ((cpuid(0x1) & 0x40) >> 6)
#endif