/**
 * $Id: boot.S 471 2011-02-22 10:38:08Z klugeflo $
 */

/*****************************************************************************/	
#define __ASSEMBLY__

#include <spr-defs.h>
#include <or32-asm.h>
	
#include <board.h>

#include <ptrace.h>

/*****************************************************************************/	

#ifndef UNHANDLED_EXCEPTION
#warning Defining empty handler for UNHANDLED_EXCEPTION!
#define UNHANDLED_EXCEPTION(name, addr)\
	l.j	0		    ;\
	l.nop
#endif


/*****************************************************************************/	


/*
Common hook for handling exceptions ; saves all registers to the stack
and jumps to #handler, which must do the actual exception handling and
may also call architecture independant handlers. The #handler has finally
to jump to _ret_from_exception
*/
#define HANDLE_EXCEPTION(handler) \
	.global handler ## _vec		      ;\
handler ## _vec:			      ;\
	l.addi	r1, r1, -(SC_STACKFRAME_SIZE) ;\
	l.sw    GPR12(r1), r12		  ;\
	l.mfspr r12, r0, SPR_ESR_BASE	  ;\
	l.sw    SR(r1), r12		  ;\
	l.mfspr r12, r0, SPR_EPCR_BASE	  ;\
	l.sw    PC(r1), r12		  ;\
	l.sw 	GPR2(r1), r2		  ;\
	l.sw	GPR3(r1), r3		  ;\
	l.sw	GPR4(r1), r4		  ;\
	l.sw	GPR5(r1), r5		  ;\
	l.sw	GPR6(r1), r6		  ;\
	l.sw	GPR7(r1), r7		  ;\
	l.sw	GPR8(r1), r8		  ;\
	l.sw	GPR9(r1), r9		  ;\
	l.sw	GPR10(r1), r10		  ;\
	l.sw	GPR11(r1), r11		  ;\
	/* r12 already safe */		  ;\
	l.sw	GPR13(r1), r13	   ;\
	l.sw	GPR14(r1), r14	   ;\
	l.sw	GPR15(r1), r15	;\
	l.sw	GPR16(r1), r16	;\
	l.sw	GPR17(r1), r17	;\
	l.sw	GPR18(r1), r18	;\
	l.sw	GPR19(r1), r19	;\
	l.sw	GPR20(r1), r20	;\
	l.sw	GPR21(r1), r21	;\
	l.sw	GPR22(r1), r22	;\
	l.sw	GPR23(r1), r23	;\
	l.sw	GPR24(r1), r24	;\
	l.sw	GPR25(r1), r25	;\
	l.sw	GPR26(r1), r26	;\
	l.sw	GPR27(r1), r27	;\
	l.sw	GPR28(r1), r28	;\
	l.sw	GPR29(r1), r29	;\
	l.sw	GPR30(r1), r30	;\
	l.sw	GPR31(r1), r31	;\
	l.ori	r31, r0, SPR_SR_SM;\
	l.mtspr r0, r31, SPR_ESR_BASE ;\
	LOAD_SYMBOL_2_GPR(r31, handler) ;\
	l.mtspr	r0, r31, SPR_EPCR_BASE	;\
	l.rfe				;\
	
	


/*****************************************************************************/	

	.org	0x0
	
	l.j	_start
	l.nop
	
	.org	0xfc
	l.nop

	
/*****************************************************************************/	

        .org    0x100           # The reset routine goes at 0x100
        .global _start

_start:
	l.movhi r1, hi(__USTACK_BOTTOM) # set SP from linker script
	l.ori	r1, r1, lo(__USTACK_BOTTOM)
	
        l.addi  r2,r1,0x0	        # FP and SP are the same

        l.mfspr r3,r0,SPR_SR     	# Get SR value
        l.ori   r3,r3,SPR_SR_IEE	# Set external exception enable bit
        l.ori   r3,r3,SPR_SR_TEE	# Set timer exception enable bit
	
	l.jal   _init                   # Jump to main routine
        l.mtspr r0,r3,SPR_SR            # Enable exceptions (DELAY SLOT)


        .org    0x1fc
        l.nop                   # Guarantee the exception vector space
        			# does not have general purpose code


/*****************************************************************************/	

/*
	Bus Error Exception
*/
	.org 0x200
	UNHANDLED_EXCEPTION(bus_error, 0x2)
	.org 0x2fc
	l.nop


/*****************************************************************************/	

/*
	Data Page Fault Exception
*/
	.org 0x300
	UNHANDLED_EXCEPTION(data_page_fault, 0x3)
	.org 0x3fc
	l.nop


/*****************************************************************************/	

/*
	Instruction Page Exception
*/
	.org 0x400
	UNHANDLED_EXCEPTION(instruction_page_fault, 0x4)
	.org 0x4fc
	l.nop


/*****************************************************************************/	

/*
	Tick Timer Exception
*/
	.org 0x500
	HANDLE_EXCEPTION(tick_timer)
	.org 0x5fc
	l.nop


/*****************************************************************************/	

/*
	Alignment Exception
*/
	.org 0x600
	UNHANDLED_EXCEPTION(alignment, 0x6)
	.org 0x6fc
	l.nop


/*****************************************************************************/	

/*
	Illegal Instruction Exception
*/
	.org 0x700
	UNHANDLED_EXCEPTION(illegal_instruction, 0x7)
	.org 0x7fc
	l.nop

	
/*****************************************************************************/	

/*
	External Interrupt Exception
*/
	.org 0x800
	HANDLE_EXCEPTION(external_interrupt)

	.org 0x8fc
	l.nop

	
/*****************************************************************************/	

/*
	Data TLB Miss Exception
*/
	.org 0x900
	UNHANDLED_EXCEPTION(d_tlb_miss, 0x9)
	.org 0x9fc
	l.nop


/*****************************************************************************/	

/*
	Instruction TLB Miss Exception
*/
	.org 0xa00
	UNHANDLED_EXCEPTION(i_tlb_miss, 0xa)
	.org 0xafc
	l.nop


/*****************************************************************************/	

/*
	Range Exception
*/
	.org 0xb00
	UNHANDLED_EXCEPTION(range, 0xb)
	.org 0xbfc
	l.nop


/*****************************************************************************/	

/*
	System Call Exception
*/
	.org 0xc00
	//HANDLE_EXCEPTION(syscall_handler)
	UNHANDLED_EXCEPTION(system_call, 0xc)
	.org 0xcfc
	l.nop


/*****************************************************************************/	

/*
	Floating Point Exception
*/
	.org 0xd00
	UNHANDLED_EXCEPTION(floating_point, 0xd)
	.org 0xdfc
	l.nop


/*****************************************************************************/	

/*
	Trap Exception
*/
	.org 0xe00
	UNHANDLED_EXCEPTION(trap, 0xe)
	.org 0xefc
	l.nop


/*****************************************************************************/	

/*
	reserved for future use
*/
	.org 0xFFC
	l.nop


	
/*****************************************************************************/	

