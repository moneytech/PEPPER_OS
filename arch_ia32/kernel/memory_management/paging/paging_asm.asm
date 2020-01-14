global _EnablingPaging_
extern __PAGE_DIRECTORY__


section .text
_EnablingPaging_:
    mov eax ,__PAGE_DIRECTORY__
    mov cr3  , eax 
    mov eax , cr0
    or eax , 0x80000000
    mov cr0 , eax
    ret