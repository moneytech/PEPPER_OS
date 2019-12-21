global protection_fault_handler

extern protection_fault

section .text



protection_fault_handler :
    pop eax ;take Error code and analyse it
    push eax
    call protection_fault
    iret


