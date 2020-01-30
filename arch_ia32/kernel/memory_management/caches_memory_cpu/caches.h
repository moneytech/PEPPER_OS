#ifndef _MEMORY_CACHES_
#define _MEMORY_CACHES
/*
    The memory type range registers (MTRRs) provide a mechanism for 
    associating the memory types with physical-address ranges in system memory.
    They allow the processor to optimize operations for different types of memory such as RAM, 
    ROM, frame-buffer memory, and memory-mapped I/O devices.
*/
void InitRangesMemory();

#include "../../../stdlib/lib.h"

#define IA32_MTRRCAP_Address 0xFE

/*
    Default Memory Types (R/W) Sets the memory type for the 
    regions of physical memory that are not mapped by the MTRRs.
*/
#define IA32_MTRR_DEF_TYPE_Address 0x2FF

/*
    If the MTRR flag is set (indicating that the processor implements MTRRs), additional information about MTRRs can
    be obtained from the 64-bit IA32_MTRRCAP MSR (named MTRRcap MSR for the P6 family processors). The
    IA32_MTRRCAP MSR is a read-only MSR that can be read with the RDMSR instruction.
*/

#define IA32_MTRRCAP (ReadMSR(IA32_MTRRCAP_Address)[0])

/*
    The memory ranges and the types of memory 
    specified in each range are set by three groups of registers:
        ->  the IA32_MTRR_DEF_TYPE MSR
        ->  the fixed-range MTRRs
        ->  and the variable range MTRRs
*/

//  -----------     the IA32_MTRR_DEF_TYPE MSR
/*
    The IA32_MTRR_DEF_TYPE MSR (named MTRRdefType MSR for the P6 family processors) 
    sets the default properties of the regions of physical memory that are not encompassed by MTRRs.
*/
#define IA32_MTRR_DEF_TYPE_set(_data_) (SetMSR(IA32_MTRR_DEF_TYPE_Address, _data_))
#define IA32_MTRR_DEF_TYPE_read (ReadMSR(IA32_MTRR_DEF_TYPE_Address)[0])
//  -------------------------------------------

//  -----------     the fixed-range MTRRs
/*
    The fixed memory ranges are mapped with 11 fixed-range registers of 64 bits each. Each of these registers is
    divided into 8-bit fields that are used to specify the memory type for each of the sub-ranges the register controls:

        ->Register IA32_MTRR_FIX64K_00000 — Maps the 512-KByte address range from 0H to 7FFFFH. This range
            is divided into eight 64-KByte sub-ranges.

        ->Registers IA32_MTRR_FIX16K_80000 and IA32_MTRR_FIX16K_A0000 — Maps the two 128-KByte
            address ranges from 80000H to BFFFFH. This range is divided into sixteen 16-KByte sub-ranges, 8 ranges per
            register.

        ->Registers IA32_MTRR_FIX4K_C0000 through IA32_MTRR_FIX4K_F8000 — Maps eight 32-KByte
            address ranges from C0000H to FFFFFH. This range is divided into sixty-four 4-KByte sub-ranges, 8 ranges per
            register.
*/

#endif