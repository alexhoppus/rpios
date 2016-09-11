.section ".text.boot"

.globl _start
_start:
	ldr pc,add_handler_00
	ldr pc,add_handler_04
	ldr pc,add_handler_08
	ldr pc,add_handler_0C
	ldr pc,add_handler_10
	ldr pc,add_handler_14
	ldr pc,add_handler_18
	ldr pc,add_handler_1C
add_handler_00: .word reset
add_handler_04: .word handler_04
add_handler_08: .word handler_08
add_handler_0C: .word handler_0C
add_handler_10: .word handler_10
add_handler_14: .word handler_14
add_handler_18: .word handler_18
add_handler_1C: .word handler_1C

#define DAB_MODE  0x17
#define UND_MODE  0x1B
#define HYP_MODE  0x1A
#define SVC_MODE  0x13
#define IRQ_MODE  0x12
#define FIQ_MODE  0x11

#define I_BIT (1 << 7)
#define F_BIT (1 << 6)

.macro setup_stack mode, base_stack
	msr cpsr, \mode
	mov sp, \base_stack
.endm

reset:
	// Assume we are in HYP mode
	mrs	r0, cpsr
	bic	r0, r0, #0x1F				
	orr	r0, r0, #SVC_MODE|I_BIT|F_BIT
	orr	r0, r0, #0x100
	adr	lr, 3f
	msr	spsr_cxsf, r0
	msr 	ELR_hyp, lr
	// Switch to SVC mode
	eret							     
3:

	// Setup stack for modes
	setup_stack #UND_MODE|I_BIT|F_BIT, #0x2000
	setup_stack #DAB_MODE|I_BIT|F_BIT, #0x4000
	setup_stack #SVC_MODE|I_BIT|F_BIT, #0x8000

	mov sp, #0x8000
	// Set vector address in CP15 VBAR register
	mov r0, #0x8000
	mcr p15, 0, r0, c12, c0, 0

	// Clear out bss.
	ldr r4, =__bss_start
	ldr r9, =__bss_end
	mov r5, #0
	mov r6, #0
	mov r7, #0
	mov r8, #0
	b       5f
 
4:
	stmia r4!, {r5-r8}
5:
	cmp r4, r9
	blo 4b
 
	// Call uart bootloader main
	ldr r3, =bootloader_main
	blx r3
halt:
	wfe
	b halt

.macro vec_handler num
handler_\num:
	mov r0,#0x\num
	ldr r3,=exception_handler
	bx r3
.endm

vec_handler 04
vec_handler 08
vec_handler 0C
vec_handler 10
vec_handler 14
vec_handler 18
vec_handler 1C
