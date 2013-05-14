/**
 * $Id: init.S 471 2011-02-22 10:38:08Z klugeflo $
 */

#define __ASSEBMLY__
	
	.global _init
_init:	



	/* handle clear table (i.e., fill BSS with zeros) */


	l.movhi	r3, hi(__clear_table)		# r3 = &first table entry
	l.ori	r3, r3, lo(__clear_table)

__clear_table_next:
	l.lwz	r14, 0x0(r3)			# r14 = current block base
	l.lwz	r5, 0x4(r3)			# r5 = current block length
	l.sfeqi	r5, -1				# length == -1?
	l.bf	__clear_table_done		#  => finished
	l.addi	r3, r3, 8			# advance block pointer
	l.srli	r6, r5, 2			# r6 = length / 4 (words)
	l.andi	r7, r5, 3			# r7 = length % 4 (rem. bytes)
	l.sfeqi r6, 0				# block size < 4
	l.bf	__clear_hword			#  => clear hword
	l.nop
	l.slli	r8, r6, 2			# number of bytes in word block
	l.add	r8, r8, r14			# end address of word block
	
__clear_word:
	l.sw	0x0(r14), r0			# clear one word
	l.addi	r14, r14, 4			# advance pointer
	l.sfeq	r14, r8				# reached end of word block?
	l.bnf	__clear_word			# if not, clear again
	l.nop

__clear_hword:
	l.sfeqi	r7, 0				# are there remaining bytes?
	l.bf	__clear_table_next		# if not => next table
	l.nop
	l.srli	r6, r7, 1			# r6 = length / 2 (hwords)
	l.andi	r7, r7, 1			# r7 = length % 2 (rem. bytes)
	l.sfeqi	r6, 0				# block size < 2
	l.bf	__clear_byte			#  => clear byte
	l.nop
	l.sh	0x0(r14), r0			# clear one hword
	l.addi	r14, r14, 2			# advance pointer

__clear_byte:
	l.sfeqi r7, 0				# remaining byte?
	l.bf	__clear_table_next		#  => done
	l.nop
	l.sb	0x0(r14), r0			# clear one byte
	l.j	__clear_table_next
	l.nop
__clear_table_done:





	/* handle copy table (support for romable code) */


	l.movhi	r3, hi(__copy_table)		# r3 = &first table entry
	l.ori	r3, r3, lo(__copy_table)
	

__copy_table_next:
	l.lwz	r15, 0x0(r3)			# r15 = src address
	l.lwz	r14, 0x4(r3)			# r14 = dst address
	l.lwz	r5, 0x8(r3)			# r5 = block length
	l.sfeqi r5, -1				# length == -1?
	l.bf	__copy_table_done		#  => finished
	l.addi	r3, r3, 12			# advance block pointer
	l.srli	r6, r5, 2			# r6 = length / 4 (words)
	l.andi	r7, r5, 3			# r7 = length % 4 (rem. bytes)
	l.sfeqi	r6, 0				# block size < 4
	l.bf	__copy_hword			#  => copy hword
	l.nop
	l.slli	r8, r6, 2			# number of bytes in word block
	l.add	r8, r8, r14			# end address of dst word block
	
__copy_word:
	l.lwz	r16, 0x0(r15)
	l.sw	0x0(r14), r16			# copy one word
	l.addi	r15, r15, 4			# advance counters
	l.addi	r14, r14, 4
	l.sfeq	r14, r8				# reached end of word block?
	l.bnf	__copy_word			# if not, copy again
	l.nop
	
__copy_hword:
	l.sfeqi	r7, 0				# are there remaining bytes?
	l.bf	__copy_table_next		# if not => next table
	l.nop
	l.srli	r6, r7, 1			# r6 = length / 2 (hwords)
	l.andi	r7, r7, 1			# r7 = length % 2 (rem. bytes)
	l.sfeqi	r6, 0				# block size < 2
	l.bf	__copy_byte			#  => clear byte
	l.nop
	l.lhz	r16, 0x0(r15)
	l.sh	0x0(r14), r16			# copy one hword
	l.addi	r15, r15, 2			# advance pointer
	l.addi	r14, r14, 2			# advance pointer

__copy_byte:
	l.sfeqi r7, 0				# remaining byte?
	l.bf	__copy_table_next		#  => done
	l.nop
	l.lbz	r16, 0x0(r15)
	l.sb	0x0(r14), r16			# copy one byte
	l.j	__copy_table_next		# handle next copy table entry
	l.nop
__copy_table_done:




	/* need to init exception vectors */


//      d7 e1 4f fc 	l.sw -4(r1),r9      # save link reg
//	04 00 00 02 	l.jal 2             # get current program counter
//      15 00 00 00     l.nop       
//	a5 29 0f 00 	l.andi r9,r9,0xf00  # get exception vector
//	44 00 48 00 	l.jr r9             # jump to
//     	85 21 ff fc 	l.lwz r9,-4(r1)     # restore link reg
//      15 00 00 00     l.nop        

// TODO! Need to adjust adresses!


        l.ori  	r3,r0,0xd         # set 0xd vectors
	l.movhi r4, 0xff00        # base address for vec hi
	l.ori	r4, r4, 0x200     # base address for vec lo

        // set above opcodes
        l.movhi	r13,0xd7e1    # save lr
        l.ori  	r13,r13,0x4ffc
	l.movhi	r5, 0x0400    # l.jal 2    
        l.ori  	r5,r5,2
	l.movhi	r6, 0x1500    # nop    
        l.ori	r6,r6,0
	l.movhi	r7, 0xa529    # l.andi    
        l.ori	r7,r7,0x0f00
	l.movhi	r8, 0x4400    # l.jr       
        l.ori	r8,r8,0x4800
        l.movhi r15,0x8521    # restore lr
        l.ori	r15,r15,0xfffc 

_copy_exvec_loop:
        l.sfleu r3,r0             # done?
        l.bf    _copy_exvec_loop_done  # yes, jmp after loop
        l.addi  r3,r3,0xffff      # counter -1 (DELAY SLOT)
        l.sw    0x0(r4), r6       # nop, 'cause board ignores first opcode
        l.sw    0x4(r4), r13      # save lr
        l.sw    0x8(r4), r5       # get current pc to lr
        l.sw    0xc(r4), r6       # nop (jump delay slot)
        l.sw    0x10(r4),r7       # mask out original exvec adress
        l.sw    0x14(r4),r8       # jump to exvec
        l.sw    0x18(r4),r15      # delay slot - restore sysvec
        l.sw    0x1c(r4),r6       # nop
        l.j     _copy_exvec_loop  # loop again
        l.addi  r4,r4,0x100       # adjust address (DELAY SLOT)
_copy_exvec_loop_done:               
        // trampoline inserted        



	/* TODO: execute constructors? */	


	/* now jump to main */


	l.add	r3, r0, r0	# argc = 0
	l.add	r4, r0, r0	# argv = 0
	l.jal   _main           # Jump to main routine
	l.nop
	l.j	0		# hang up
	l.nop

