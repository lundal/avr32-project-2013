/*
 * EVBA vector and lowlevel interrupt handling for
 * libavr32b-interrupts - a library to emulate an older version
 * of avr32b framework.
 *
 * written by Ole Henrik Jahren <olehenja@alumni.ntnu.no>
 */

	.section .text,"ax",@progbits
	.balign 0x200
	.global _evba
_evba:
	/* AVR32_EVBA_OFFSET_UNRECOVERABLE */
	.org 0x00000000
	rjmp panic
	/* AVR32_EVBA_OFFSET_TLB_MULTIPLE */
	.org 0x00000004
	rjmp panic
	/* AVR32_EVBA_OFFSET_BUS_ERROR_DATA */
	.org 0x00000008
	rjmp panic
	/* AVR32_EVBA_OFFSET_BUS_ERROR_INSTR */
	.org 0x0000000c
	rjmp panic
	/* AVR32_EVBA_OFFSET_NMI */
	.org 0x00000010 
	rjmp panic
	/* AVR32_EVBA_OFFSET_INSTR_ADDR */
	.org 0x00000014
	rjmp panic
	/* AVR32_EVBA_OFFSET_ITLB_PROT */
	.org 0x00000018
	rjmp panic
	/* AVR32_EVBA_OFFSET_BREAKPOINT */
	.org 0x0000001c
	rjmp panic
	/* AVR32_EVBA_OFFSET_ILLEGAL_OPCODE */
	.org 0x00000020
	rjmp panic
	/* AVR32_EVBA_OFFSET_UNIMPLEMENTED */
	.org 0x00000024
	rjmp panic
	/* AVR32_EVBA_OFFSET_PRIVILEGE_VIOL */
	.org 0x00000028
	rjmp panic
	/* AVR32_EVBA_OFFSET_FLOATING_POINT */
	.org 0x0000002c
	rjmp panic
	/* AVR32_EVBA_OFFSET_COP_ABSENT */
	.org 0x00000030
	rjmp panic
	/* AVR32_EVBA_OFFSET_DATA_ADDR_R */
	.org 0x00000034
	rjmp panic
	/* AVR32_EVBA_OFFSET_DATA_ADDR_W */
	.org 0x00000038
	rjmp panic
	/* AVR32_EVBA_OFFSET_DTLB_PROT_R */
	.org 0x0000003c
	rjmp panic
	/* AVR32_EVBA_OFFSET_DTLB_PROT_W */
	.org 0x00000040
	rjmp panic
	/* AVR32_EVBA_OFFSET_DTLB_MODIFIED */
	.org 0x00000044
	rjmp panic
	/* AVR32_EVBA_OFFSET_ITLB_MISS */
	.org 0x00000050
	rjmp panic
	/* AVR32_EVBA_OFFSET_DTLB_MISS_R */
	.org 0x00000060
	rjmp panic
	/* AVR32_EVBA_OFFSET_DTLB_MISS_W */
	.org 0x00000070
	rjmp panic
	/* AVR32_EVBA_OFFSET_SCALL */
	.org 0x00000100
	rjmp panic

	.macro IRQ_LEVEL level
	.global _irq_level\level
_irq_level\level:
	stmts	--sp, r0-lr
	mov	r12, \level
	rcall	_do_irq
	ldmts	sp++, r0-lr
	rete
	.endm

	IRQ_LEVEL 0
	IRQ_LEVEL 1
	IRQ_LEVEL 2
	IRQ_LEVEL 3
