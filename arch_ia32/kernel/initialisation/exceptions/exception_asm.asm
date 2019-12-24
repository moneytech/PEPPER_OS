global _general_protection_fault_handler_

extern exception

section .text

;handler|eflags|cs:eip|error code
_general_protection_fault_handler_ :
   pop eax
   mov dword [_error_code_] , eax
    call exception
  iret
    
section .data 

    _error_code_ dd 0