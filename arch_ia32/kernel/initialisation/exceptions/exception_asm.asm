global protection_fault_handler

extern protection_fault

section .text

;eflags|cs:eip|error code
protection_fault_handler :
    pop eax
    mov dword [_error_code_] , eax
    call protection_fault
    iret
    
section .data 

    _error_code_ dd 0