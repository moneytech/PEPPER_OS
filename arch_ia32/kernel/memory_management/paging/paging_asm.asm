global _EnablingPaging_
extern page_directory  , Paging_fault


section .text
_EnablingPaging_:
    mov eax ,page_directory
    mov cr3  , eax 
    mov eax , cr0
    or eax , 0x80000000
    mov cr0 , eax
    ret

PagingFault_Handler:
    pop eax
    mov dword[error_code] , eax
    call Paging_fault
    iret

    section .data
        error_code dd 0
