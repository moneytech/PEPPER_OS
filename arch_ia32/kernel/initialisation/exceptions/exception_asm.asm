global exception_handler

extern exception

section .text

<<<<<<< HEAD:arch_ia32/kernel/initialisation/exceptions/exception_asm.asm
;eflags|cs:eip|error code
protection_fault_handler :
    pop eax
    mov dword [_error_code_] , eax
    call protection_fault
=======

;eflags|cs:eip|error code
exception_handler :
    pop eax ;take Error code and analyse it
    push eax
    call exception
    pop eax
>>>>>>> ecab81465069d2e21f3c6f2af2ed85c8cf8ca835:arch_ia32/kernel/initialisation/exceptions/exception_err_code_asm.asm
    iret
    
section .data 

    _error_code_ dd 0