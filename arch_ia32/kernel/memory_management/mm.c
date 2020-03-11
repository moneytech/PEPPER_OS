
    #include "mm.h"
    #include "../../stdlib/i386types.h"

virtaddr_t* alloc_page(uint32_t zone, uint32_t order)
{
    _address_order_track_ *tmp =(_address_order_track_*) _page_area_track_ ,
        *new_entry_page ;

    //Si la liste est vide
    if ((_page_area_track_->next_ == END_LIST) && (_page_area_track_->_address_ == NO_PHYSICAL_ADDRESS))
   _page_area_track_->_address_ = PAGES_MEMORY_ZONE ;

    //Si elle est unique dans la liste
    else if (tmp->next_ == END_LIST)
    {
        _page_area_track_->next_ = new_entry_page ;
        new_entry_page->next_ = END_LIST ;
        new_entry_page->previous_ = _page_area_track_ ;
        new_entry_page->order = order ;
        new_entry_page->_address_ = (physaddr_t*) ( (uint32_t)(_page_area_track_->_address_)+(0x1000 * order));
        return (virtaddr_t*)(new_entry_page->_address_) ;
    }

    //si la liste contient au mons 2 éléments
    else
    {
        while (tmp->next_ != END_LIST)
        {
            if ((uint32_t) (tmp->next_->_address_) >= (uint32_t)((uint32_t)(tmp->_address_) + (uint32_t)((tmp->order + order) * 0x1000)))
            {
                new_entry_page->_address_ = (physaddr_t*)((uint32_t)(tmp->_address_)+(uint32_t)(tmp->order)*0x1000) ;
                new_entry_page->order = order ;

                new_entry_page->previous_ = tmp ;
                tmp->next_->previous_ = new_entry_page ;
                new_entry_page->next_ = tmp->next_ ;
                tmp->next_ = new_entry_page ;
                
                return (virtaddr_t*)new_entry_page->_address_ ;
            }

            tmp = tmp->next_ ;
        }
    }
}

void (free_page)(uint32_t zone , virtaddr_t* addr)
{
    physaddr_t *phy_addr = (get_phyaddr)(addr) ;

    //Si nous sommes en tête de list
    if (_page_area_track_->_address_ == phy_addr)
    {
        _page_area_track_= _page_area_track_->next_ ;

        _page_area_track_->previous_ = END_LIST ;

        //si elles est seule dans la liste
        if (_page_area_track_->next_ == (_address_order_track_*)(END_LIST))

        _page_area_track_->_address_ = NO_PHYSICAL_ADDRESS ;

        return ;
    }

    else 
    {
        _address_order_track_ *tmp =(_address_order_track_*) _page_area_track_ ;

        while (tmp->next_ != END_LIST)
        {
            if (tmp->_address_ == phy_addr)
            {
                tmp->previous_->next_ = tmp->next_ ;
                return ;
            }
            tmp = tmp->next_ ;
        } 
    }   
}

