/**
 * $Id: nios2intrinsics.h 672 2013-04-12 10:30:44Z klugeflo $
 * NIOS2 intrinsics
 */

#ifndef NIOS2INTRINSICS_H
#define NIOS2INTRINSICS_H 1

#define NOP "\tnop"

#define __rdctl_status()                                    \
    ({ uint32_t val;                                        \
        asm volatile ("rdctl %0, status" : "=r"(val) : );   \
        val; })
#define __rdctl_estatus()                                   \
    ({ uint32_t val;                                        \
        asm volatile ("rdctl %0, estatus" : "=r"(val) : );  \
        val; })
#define __rdctl_bstatus()                                   \
    ({ uint32_t val;                                        \
        asm volatile ("rdctl %0, bstatus" : "=r"(val) : );  \
        val; })
#define __rdctl_ienable()                                   \
    ({ uint32_t val;                                        \
        asm volatile ("rdctl %0, ienable" : "=r"(val) : );  \
        val; })
#define __rdctl_ipending()                                  \
    ({ uint32_t val;                                        \
        asm volatile ("rdctl %0, ipending" : "=r"(val) : ); \
        val; })
#define __rdctl_cpuid()                                     \
    ({ uint32_t val;                                        \
        asm volatile ("rdctl %0, cpuid" : "=r"(val) : );    \
        val; })

#define __rdctl_badaddr()                                   \
    ({ uint32_t val;                                        \
        asm volatile ("rdctl %0, badaddr" : "=r"(val) : );  \
        val; })
#define __rdctl_exception()                                 \
    ({ uint32_t val;                                        \
        asm volatile ("rdctl %0, exception" : "=r"(val) : );\
        val; })


#define __wrctl_status(val)                             \
    { asm volatile ("wrctl status, %0" : : "r"(val)); }
#define __wrctl_ienable(val)                            \
    { asm volatile ("wrctl ienable, %0" : : "r"(val)); }
#define __wrctl_ipending(val) 0

#endif /* NIOS2_NIOS2INTRINSICS_H */
