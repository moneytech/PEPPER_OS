global _general_protection_handler_ , _error_code_

extern _general_protection_

section .text
    
    _general_protection_handler_ :
        call _general_protection_
        pop eax
        iret

section .data 

    _error_code_ dd 0