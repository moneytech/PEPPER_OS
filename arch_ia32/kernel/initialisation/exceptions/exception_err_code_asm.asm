global exception_handler

extern protection_fault

section .text


;eflags|cs:eip|error code
exception_handler :
    pop eax ;take Error code and analyse it
    push eax
    call exception
    pop eax
    iret


