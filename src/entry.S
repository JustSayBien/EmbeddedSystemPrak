/**
 * $Id: entry.S 471 2011-02-22 10:38:08Z klugeflo $
 */

/******************************************************************************

This file defines the entry points for all exceptions, and some additional
functions like context switching

******************************************************************************/	

#define __ASSEMBLY__

#include <spr-defs.h>
#include <ptrace.h>
#include <board.h>
#include <or32-asm.h>

/*****************************************************************************/	

// TODO: remove board dependency!
/*	
#define OUTPUT_AND_HALT(gprA, gprB, val)\
	LOAD_SYMBOL_2_GPR(gprA, A_SEVEN_SEG) ;\
	LOAD_SYMBOL_2_GPR(gprB, val) ;\
	l.sw	0x0(gprA), gprB	;\
	l.j	0		;\
	l.nop

#define OUTPUT_REG_AND_HALT(gprA, gprV) \
	LOAD_SYMBOL_2_GPR(gprA, A_SEVEN_SEG) ;\
	l.sw	0x0(gprA), gprV	;\
	l.j	0		;\
	l.nop
*/
/*****************************************************************************/	
	
#define ENTRY(handler)				\
	.global	handler						;\
handler:

	
/*****************************************************************************/	

#define EXCEPTION_RESTORE\
	l.lwz r12,SR(r1)		;\
	l.mtspr r0,r12,SPR_ESR_BASE 	;\
	l.lwz r12,PC(r1)		;\
	l.mtspr r0,r12,SPR_EPCR_BASE 	;\
	l.lwz r2,GPR2(r1)		;\
	l.lwz r3,GPR3(r1)		;\
	l.lwz r4,GPR4(r1)		;\
	l.lwz r5,GPR5(r1)		;\
	l.lwz r6,GPR6(r1)		;\
	l.lwz r7,GPR7(r1)		;\
	l.lwz r8,GPR8(r1)		;\
	l.lwz r9,GPR9(r1)		;\
	l.lwz r10,GPR10(r1)		;\
	l.lwz r11,GPR11(r1)		;\
	l.lwz r12,GPR12(r1)		;\
	l.lwz r13,GPR13(r1)		;\
	l.lwz r14,GPR14(r1)		;\
	l.lwz r15,GPR15(r1)		;\
	l.lwz r16,GPR16(r1)		;\
	l.lwz r17,GPR17(r1)		;\
	l.lwz r18,GPR18(r1)		;\
	l.lwz r19,GPR19(r1)		;\
	l.lwz r20,GPR20(r1)		;\
	l.lwz r21,GPR21(r1)		;\
	l.lwz r22,GPR22(r1)		;\
	l.lwz r23,GPR23(r1)		;\
	l.lwz r24,GPR24(r1)		;\
	l.lwz r25,GPR25(r1)		;\
	l.lwz r26,GPR26(r1)		;\
	l.lwz r27,GPR27(r1)		;\
	l.lwz r28,GPR28(r1)		;\
	l.lwz r29,GPR29(r1)		;\
	l.lwz r30,GPR30(r1)		;\
	l.lwz r31,GPR31(r1)		;\
	l.addi r1, r1, (SC_STACKFRAME_SIZE) ;\
	l.rfe				;\
	l.nop

	
/*****************************************************************************/	

// need to change this!
#define RESTORE_ALL_REGS	;\
	l.lwz r12,SR(r1)	;\
	l.mtspr r0,r12,SPR_ESR_BASE ;\
	l.lwz r12,PC(r1)	    ;\
	l.mtspr r0,r12,SPR_EPCR_BASE ;\
	l.lwz r2,GPR2(r1)	     ;\
	l.lwz r3,GPR3(r1)	     ;\
	l.lwz r4,GPR4(r1)	     ;\
	l.lwz r5,GPR5(r1)	     ;\
	l.lwz r6,GPR6(r1)	     ;\
	l.lwz r7,GPR7(r1)	     ;\
	l.lwz r8,GPR8(r1)	     ;\
	l.lwz r9,GPR9(r1)	     ;\
	l.lwz r10,GPR10(r1)	     ;\
	l.lwz r11,GPR11(r1)	     ;\
	l.lwz r12,GPR12(r1)	     ;\
	l.lwz r13,GPR13(r1)	     ;\
	l.lwz r14,GPR14(r1)	     ;\
	l.lwz r15,GPR15(r1)	     ;\
	l.lwz r16,GPR16(r1)	     ;\
	l.lwz r17,GPR17(r1)	     ;\
	l.lwz r18,GPR18(r1)	     ;\
	l.lwz r19,GPR19(r1)	     ;\
	l.lwz r20,GPR20(r1)	     ;\
	l.lwz r21,GPR21(r1)	     ;\
	l.lwz r22,GPR22(r1)	     ;\
	l.lwz r23,GPR23(r1)	     ;\
	l.lwz r24,GPR24(r1)	     ;\
	l.lwz r25,GPR25(r1)	     ;\
	l.lwz r26,GPR26(r1)	     ;\
	l.lwz r27,GPR27(r1)	     ;\
	l.lwz r28,GPR28(r1)	     ;\
	l.lwz r29,GPR29(r1)	     ;\
	l.lwz r30,GPR30(r1)	     ;\
	l.lwz r31,GPR31(r1)	     ;\
	l.addi r1, r1, (SC_STACKFRAME_SIZE) ;\
	l.rfe


/*****************************************************************************/	

#define RESTORE_ALL_NO_R11\
	l.lwz r12,SR(r1)	;\
	l.mtspr r0,r12,SPR_ESR_BASE ;\
	l.lwz r12,PC(r1)	    ;\
	l.mtspr r0,r12,SPR_EPCR_BASE ;\
	l.lwz r2,GPR2(r1)	     ;\
	l.lwz r3,GPR3(r1)	     ;\
	l.lwz r4,GPR4(r1)	     ;\
	l.lwz r5,GPR5(r1)	     ;\
	l.lwz r6,GPR6(r1)	     ;\
	l.lwz r7,GPR7(r1)	     ;\
	l.lwz r8,GPR8(r1)	     ;\
	l.lwz r9,GPR9(r1)	     ;\
	l.lwz r10,GPR10(r1)	     ;\
	l.lwz r12,GPR12(r1)	     ;\
	l.lwz r13,GPR13(r1)	     ;\
	l.lwz r14,GPR14(r1)	     ;\
	l.lwz r15,GPR15(r1)	     ;\
	l.lwz r16,GPR16(r1)	     ;\
	l.lwz r17,GPR17(r1)	     ;\
	l.lwz r18,GPR18(r1)	     ;\
	l.lwz r19,GPR19(r1)	     ;\
	l.lwz r20,GPR20(r1)	     ;\
	l.lwz r21,GPR21(r1)	     ;\
	l.lwz r22,GPR22(r1)	     ;\
	l.lwz r23,GPR23(r1)	     ;\
	l.lwz r24,GPR24(r1)	     ;\
	l.lwz r25,GPR25(r1)	     ;\
	l.lwz r26,GPR26(r1)	     ;\
	l.lwz r27,GPR27(r1)	     ;\
	l.lwz r28,GPR28(r1)	     ;\
	l.lwz r29,GPR29(r1)	     ;\
	l.lwz r30,GPR30(r1)	     ;\
	l.lwz r31,GPR31(r1)	     ;\
	l.addi r1, r1, (SC_STACKFRAME_SIZE) ;\
	l.rfe


/*****************************************************************************/	

#define PUSH_PT_REGS_NO_TRANS					\
        l.addi  r1,r1,-(SC_STACKFRAME_SIZE)				;\
        l.sw    GPR3(r1),r3					;\
        l.mfspr r3,r0,SPR_EPCR_BASE				;\
        l.sw    PC(r1),r3					;\
        l.mfspr r3,r0,SPR_ESR_BASE				;\
        l.sw    SR(r1),r3					;\
        l.sw    GPR2(r1),r2					;\
	l.lwz	r3, GPR3(r1)					;\
	l.sw    GPR4(r1),r4					;\
        l.sw    GPR5(r1),r5					;\
        l.sw    GPR6(r1),r6					;\
        l.sw    GPR7(r1),r7					;\
        l.sw    GPR8(r1),r8					;\
        l.sw    GPR9(r1),r9					;\
        l.sw    GPR10(r1),r10					;\
        l.sw    GPR11(r1),r11					;\
        l.sw    GPR12(r1),r12					;\
        l.sw    GPR13(r1),r13					;\
        l.sw    GPR14(r1),r14					;\
        l.sw    GPR15(r1),r15					;\
        l.sw    GPR16(r1),r16					;\
        l.sw    GPR17(r1),r17					;\
        l.sw    GPR18(r1),r18					;\
        l.sw    GPR19(r1),r19					;\
        l.sw    GPR20(r1),r20					;\
        l.sw    GPR21(r1),r21					;\
        l.sw    GPR22(r1),r22					;\
        l.sw    GPR23(r1),r23					;\
        l.sw    GPR24(r1),r24					;\
        l.sw    GPR25(r1),r25					;\
        l.sw    GPR26(r1),r26					;\
        l.sw    GPR27(r1),r27					;\
        l.sw    GPR28(r1),r28					;\
        l.sw    GPR29(r1),r29					;\
        l.sw    GPR30(r1),r30					;\
        l.sw    GPR31(r1),r31					;\
        //l.sw    RESULT(r1),r0
	

/*****************************************************************************/	

/* [0x100] Reset - the init code */


/*****************************************************************************/

/* [0x200] Bus Error */


/*****************************************************************************/

/* [0x300] Data Page Fault */


/*****************************************************************************/

/* [0x400] Instruction Page Fault */


/*****************************************************************************/

/* [0x500] Tick Timer */

ENTRY(tick_timer)
	l.add   r3, r1, r0	# address of task context on stack
	l.jal	_do_timer
	l.nop
EXCEPTION_RESTORE
	

/*****************************************************************************/

/* [0x600] Alignment */


/*****************************************************************************/

/* [0x700] Illegal Instruction */


/*****************************************************************************/

/* [0x800] External Interrupt */

ENTRY(external_interrupt)
	l.add   r3, r1, r0	# address of task context on stack
	l.jal	_do_irq
	l.nop
EXCEPTION_RESTORE

	
/*****************************************************************************/

/* [0x900] D-TLB Miss */


/*****************************************************************************/

/* [0xa00] I-TLB Miss */


/*****************************************************************************/

/* [0xb00] Range */


/*****************************************************************************/

/* [0xc00] System Call */

	/*
ENTRY(syscall_handler)
	l.movhi r30,hi(_syscall_table)
	l.ori   r30,r30,lo(_syscall_table)
	l.slli  r11,r11,2
	l.add   r30,r30,r11
	l.lwz   r30,0(r30)

	l.jalr  r30 		# jump to the system service
	l.add   r8, r1, r0	# address of task SP

	l.sw	GPR11(r1), r11
	RESTORE_ALL_NO_R11
	//RESTORE_ALL_REGS
	*/

/*****************************************************************************/

/* [0xd00] Floating Point */


/*****************************************************************************/

/* [0xe00] Trap */


/*****************************************************************************/

ENTRY(_ret_from_exception)
	RESTORE_ALL_REGS

	
/*****************************************************************************/

#if 0 // no context switches for Roomba Training
/*
	Switch execution context
Semantics:
	void _switch_context(task_t *old, task_t *new)

	save/load stack pointer from/to task struct at offest TASK_KSP
*/
//	OUTPUT_REG_AND_HALT(r13, r1)		    ;

ENTRY(__switch_context)
	l.sw    (GPR3-SC_STACKFRAME_SIZE)(r1), r3
        l.mtspr r0, r9, SPR_EPCR_BASE		/* Link register to EPCR */
        l.mfspr r3, r0, SPR_SR			/* From SR to ESR */
        l.mtspr r0, r3, SPR_ESR_BASE

	l.lwz   r3,(GPR3-SC_STACKFRAME_SIZE)(r1)

	PUSH_PT_REGS_NO_TRANS
	l.sw    TASK_KSP(r3),r1			/* Save old stack pointer */


        l.lwz   r1,TASK_KSP(r4)			/* Load new stack pointer */
	RESTORE_ALL_REGS

#endif /* 0 */	 
	
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


