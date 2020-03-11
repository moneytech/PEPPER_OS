#ifndef _MM_

    #include "../../stdlib/i386types.h"
    #include "paging/paging.h"

    #define _MM_

        

typedef struct _address_order_table_ 
{
    physaddr_t* _address_ ;
    uint32_t order ;
    struct _address_order_table_ *previous_ ;
    struct _address_order_table_ *next_ ;
}_address_order_track_ ;

_address_order_track_ *_page_area_track_ ;

    #define END_LIST    ((_address_order_track_ *)("end_list"))
        

    virtaddr_t* alloc_page(uint32_t zone, uint32_t order) ;

    /*
        Free pages, 
            is a lot of simpler and exists to help remember the order of the blocks , 
            to free as one disadvantage of a buddy allocator is that the caller has 
            to remember the size of the original allocation.
    */

   //Free a page from the given virtual address
    void (free_page)(uint32_t zone , virtaddr_t *addrr) ;


#endif