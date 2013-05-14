/**
 * $Id: ptrace.h 467 2011-02-21 12:49:09Z klugeflo $
 */

#ifndef _OR32_PTRACE_H
#define _OR32_PTRACE_H

/* plucked from linux-2.6.24, adjusted for RTOS training (06.11.2009 FAK) */

//#include <spr-defs.h>

// if you change anything on the data structures below, you must also adjust
// this value!
#define SC_STACKFRAME_SIZE 128

#ifndef __ASSEMBLY__

#include <stdint.h>

typedef struct {
  uint32_t pc;
  uint32_t sr;
  uint32_t gpr2;
  uint32_t gpr3;
  uint32_t gpr4;
  uint32_t gpr5;
  uint32_t gpr6;
  uint32_t gpr7;
  uint32_t gpr8;
  uint32_t gpr9;
  uint32_t gpr10;
  uint32_t gpr11;
  uint32_t gpr12;
  uint32_t gpr13;
  uint32_t gpr14;
  uint32_t gpr15;
  uint32_t gpr16;
  uint32_t gpr17;
  uint32_t gpr18;
  uint32_t gpr19;
  uint32_t gpr20;
  uint32_t gpr21;
  uint32_t gpr22;
  uint32_t gpr23;
  uint32_t gpr24;
  uint32_t gpr25;
  uint32_t gpr26;
  uint32_t gpr27;
  uint32_t gpr28;
  uint32_t gpr29;
  uint32_t gpr30;
  uint32_t gpr31;
  //uint32_t result;
  //uint32_t syscallno;
} sc_stackframe_t;

#else


/*
 * Offsets used by 'ptrace' system call interface.
 */

/*
  You can use this defines as offsets for your load/store instructions, e.g.

l.sw  PC(r1), r9
l.sw  GPR9(r1), r9

l.lwz r3, GPR3(r1)

*/

#define PC        0
#define SR        4
#define GPR2      8
#define GPR3      12 
#define GPR4      16 
#define GPR5      20 
#define GPR6      24 
#define GPR7      28 
#define GPR8      32 
#define GPR9      36 // need not save this
#define GPR10     40 
#define GPR11     44 
#define GPR12     48 
#define GPR13     52 
#define GPR14     56 
#define GPR15     60 
#define GPR16     64 
#define GPR17     68 
#define GPR18     72 
#define GPR19     76 
#define GPR20     80 
#define GPR21     84 
#define GPR22     88 
#define GPR23     92 
#define GPR24     96 
#define GPR25     100
#define GPR26     104
#define GPR27     108
#define GPR28     112
#define GPR29     116
#define GPR30     120
#define GPR31     124
//#define RESULT    128
//#define SYSCALLNO 132

#endif /* __ASSEMBLY__ */

#define TASK_KSP 4

#endif /* _OR32_PTRACE_H */
