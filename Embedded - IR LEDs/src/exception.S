#define __ASSEMBLY__
#include <asm/ptrace.h>
//#include <driver/de2-70.h>
#include <board.h>
/*************************************************************************\
|                     Exception handler (.exceptions)                     |
\*************************************************************************/
    .set noat
    .set nobreak
    .section .exceptions.entry.label, "xa"

    .globl alt_exception
    .type alt_exception, @function
alt_exception:

    .section .exceptions
general_exception:
/* save stackframe */
    addi    sp,  sp, -SC_STACKFRAME_SIZE
    stw     r1,  GPR1(sp)
    rdctl   r1,  estatus
    stw     r1,  SR(sp)
    stw     ea,  PC(sp)
    stw     r2,  GPR2(sp)
    stw     r3,  GPR3(sp)
    stw     r4,  GPR4(sp)
    stw     r5,  GPR5(sp)
    stw     r6,  GPR6(sp)
    stw     r7,  GPR7(sp)
    stw     r8,  GPR8(sp)
    stw     r9,  GPR9(sp)
    stw     r10, GPR10(sp)
    stw     r11, GPR11(sp)
    stw     r12, GPR12(sp)
    stw     r13, GPR13(sp)
    stw     r14, GPR14(sp)
    stw     r15, GPR15(sp)
    stw     r16, GPR16(sp)
    stw     r17, GPR17(sp)
    stw     r18, GPR18(sp)
    stw     r19, GPR19(sp)
    stw     r20, GPR20(sp)
    stw     r21, GPR21(sp)
    stw     r22, GPR22(sp)
    stw     r23, GPR23(sp)
    stw     r24, GPR24(sp)
    stw     r25, GPR25(sp)
    /* global pointer: never modify
    stw     r26, GPR26(sp)*/
    /* stack pointer: do not save or load
    stw     r27, GPR27(sp)*/
    stw     r28, GPR28(sp)
    stw     r29, GPR29(sp)
    stw     r30, GPR30(sp)
    stw     r31, GPR31(sp)
/* read exception cause and store as first parameter */
    rdctl   r13, exception
    srli    r13, r13, 2
    andi    r13, r13, 0x001f
/* check for irq */
    movi    r14, 2
    beq	r13, r14, process_irq
/* check for syscall / trap exception */
    movi    r14, 3
    beq     r13, r14, process_syscall
/* set cause as first parameter */
    mov     r4,  r13
/* set saved stackframe as second parameter */
    mov     r5,  sp
/* call general exception handler */
    call    handle_ge
/* restore stackframe */
exception_restore:
    ldw     r1,  SR(sp)
    wrctl   estatus, r1
    ldw     r1,  GPR1(sp)
    ldw     r2,  GPR2(sp)
    ldw     r3,  GPR3(sp)
    ldw     r4,  GPR4(sp)
    ldw     r5,  GPR5(sp)
    ldw     r6,  GPR6(sp)
    ldw     r7,  GPR7(sp)
    ldw     r8,  GPR8(sp)
    ldw     r9,  GPR9(sp)
    ldw     r10, GPR10(sp)
    ldw     r11, GPR11(sp)
    ldw     r12, GPR12(sp)
    ldw     r13, GPR13(sp)
    ldw     r14, GPR14(sp)
    ldw     r15, GPR15(sp)
    ldw     r16, GPR16(sp)
    ldw     r17, GPR17(sp)
    ldw     r18, GPR18(sp)
    ldw     r19, GPR19(sp)
    ldw     r20, GPR20(sp)
    ldw     r21, GPR21(sp)
    ldw     r22, GPR22(sp)
    ldw     r23, GPR23(sp)
    ldw     r24, GPR24(sp)
    ldw     r25, GPR25(sp)
    /* global pointer: never modify
    ldw     r26, GPR26(sp)*/
    /* stack pointer: do not save or load
    ldw     r27, GPR27(sp)*/
    ldw     r28, GPR28(sp)
    ldw     r29, GPR29(sp)
    ldw     r30, GPR30(sp)
    ldw     r31, GPR31(sp)
    ldw     ea,  PC(sp)
    addi    sp,  sp, SC_STACKFRAME_SIZE
    eret

process_irq:
    /* copy saved stackframe as first parameter */
    mov     r4, sp
    /* call irq handler */
    call    do_irq

    /* return to exception handler */
    br      exception_restore

process_syscall:
    /* calculate syscall address */
    slli    r13, r11, 2
    movhi   r14, %hi(_syscall_table)
    addi    r14, r14, %lo(_syscall_table)
    add     r14, r14, r13
    ldw     r15, 0(r14)
    movhi   r16, %hi(A_SEVEN_SEG)
    addi    r16, r16, %lo(A_SEVEN_SEG)
    stw     r15, 0(r16)

    /* Copy saved stackframe as 6th parameter
        * 5th parameter is passed as 0(sp)
        * 6th parameter is passed as 4(sp) */
    mov     r9, sp
    subi    sp, sp, 8
    stw     r8, 0(sp)
    stw     r9, 4(sp)

    /* execute syscall */
    callr   r15

    /* remove parameters from stack */
    addi    sp, sp, 8

    /* store return value
        * syscall macros use r11 */
    stw     r2, GPR11(sp)

    /* return to exception handler */
    br  exception_restore

#ifdef USE_OS	

#ifdef OWN_OS
    .globl _syscall_table
_syscall_table:
    /* 0x0*: Task management */
    .long _my_sc_activate_task
    .long _my_sc_terminate_task
    .long _my_sc_chain_task
    .long _my_sc_schedule
    .long _my_sc_get_task_id
    .long _my_sc_get_task_state
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    /* 0x1*: Event management */
    .long _my_sc_set_event
    .long _my_sc_clear_event
    .long _my_sc_get_event
    .long _my_sc_wait_event
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    /* 0x2*: Resources */
    .long _my_sc_get_resource
    .long _my_sc_release_resource
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    /* 0x3*: OS execution control */
    .long _my_sc_set_os_nonpreemptable
    .long _my_sc_set_os_preemptable
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    /* 0x4*: Interrupts */
    .long _my_sc_enable_all_interrupts
    .long _my_sc_disable_all_interrupts
    .long _my_sc_irq_enable
    .long _my_sc_irq_disable
    .long _my_sc_irq_request
    .long _my_sc_irq_free   
    .long _my_sc_irq_enabled
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    /* 0x5*: Timer */
    .long _my_sc_tt_register
    .long _my_sc_tt_periodic
    .long _my_sc_tt_single

#else /* OWN_OS */

    .globl _syscall_table
_syscall_table: 
    /* 0x0*: Task management */
    .long _sc_activate_task
    .long _sc_terminate_task
    .long _sc_chain_task
    .long _sc_schedule
    .long _sc_get_task_id
    .long _sc_get_task_state
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    /* 0x1*: Event management */
    .long _sc_set_event
    .long _sc_clear_event
    .long _sc_get_event
    .long _sc_wait_event
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    /* 0x2*: Resources */
    .long _sc_get_resource
    .long _sc_release_resource
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    /* 0x3*: OS execution control */
    .long _sc_set_os_nonpreemptable
    .long _sc_set_os_preemptable
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    /* 0x4*: Interrupts */
    .long _sc_enable_all_interrupts
    .long _sc_disable_all_interrupts
    .long _sc_irq_enable
    .long _sc_irq_disable
    .long _sc_irq_request
    .long _sc_irq_free   
    .long _sc_irq_enabled
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    /* 0x5*: Timer */
    .long _sc_tt_register
    .long _sc_tt_periodic
    .long _sc_tt_single

#endif /* OWN_OS */

#endif /* USE_OS */

