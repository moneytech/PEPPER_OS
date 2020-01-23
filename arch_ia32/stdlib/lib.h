
#ifndef LIB_h

#define LIB_h

#include "i386types.h"

//fonction de copie de n octet
void memcpy(int8_t *dest, int8_t *src, uint32_t i);

#define cpuid(code) ({                 \
    uint32_t edx;                      \
    __asm__ __volatile__("cpuid"       \
                         : "=d"(edx)   \
                         : "a"(code)); \
    edx;                               \
})

#define cpuidAllRegisters(code) ({             \
    uint32_t registers[0x4];                   \
    __asm__ __volatile__("cpuid"               \
                         : "=a"(registers[0]), \
                           "=b"(regiters[1]),  \
                           "=c"(registers[2]), \
                           "=d"(registers[3])  \
                         : "a"(code));         \
    registers;                                 \
})

#endif  // !LIB_h
