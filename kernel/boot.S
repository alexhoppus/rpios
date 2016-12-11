// Borrowed from Raspberry_Pi_Bare_Bones:wq

// Reference: http://wiki.osdev.org/Raspberry_Pi_Bare_Bones
// To keep this in the first portion of the binary.
.section ".text.boot"
 
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
	ldr sp, \base_stack
.endm

#define KPGD_BASE 0x20000
#define AP_KRW (0x1 << 10)
#define SECTION_TYPE 0x2
#define VIRTUAL_BASE 0xc0000000
#define SCTLR_M (1 << 0)
periph_base: .word 0x3f200000
virtual_base: .word 0xc0000000

// We assume that KPGD (kernel page dir) placed at 0x20000
// address. This functions set initial one-megabyte mapping
// 0x00000000 -> 0xc0000000
.macro create_page_table
	mov r0, #AP_KRW
	orr r1, r0, #SECTION_TYPE
	mov r3, #KPGD_BASE
	ldr r4, virtual_base
	lsr r4, r4, #20
	str r1, [r3]
	str r1, [r3, r4, lsl #2]

	ldr r4, periph_base
	ldr r1, periph_base
	lsr r4, r4, #20

	orr r1, r1, #AP_KRW
	orr r1, r1, #SECTION_TYPE
	str r1, [r3, r4, lsl #2]
.endm

// Turn MMU on
.macro enable_mmu
	mov r2, #0
	// Caches & TLB invalidation
	mcr p15, 0, r2, c8, c7, 0
	mcr p15, 0, r2, c7, c10, 4

	// For domain 0:
	// Accesses are checked against the permission
	// bits in the translation tables
	mov r2, #0x1
	mcr p15, 0, r2, c3, c0, 0

	// Setting TTBR registers
	mov r0, #KPGD_BASE
	mcr p15, 0, r0, c2, c0, 0
	mcr p15, 0, r0, c2, c0, 1

	// Enable MMU
	mrc p15, 0, r2, c1, c0, 0
	orr r2, r2, #SCTLR_M
	mcr p15, 0, r2, c1, c0, 0
.endm

.globl _start
_start:
	// Setup stack for modes
	setup_stack #UND_MODE|I_BIT|F_BIT, und_stack
	setup_stack #DAB_MODE|I_BIT|F_BIT, dab_stack
	setup_stack #SVC_MODE|I_BIT|F_BIT, svc_stack

	// Clear out bss.
	ldr r4, =__bss_start
	ldr r9, =__bss_end
	mov r5, #0
	mov r6, #0
	mov r7, #0
	mov r8, #0
	b       2f
 
1:
	// store multiple at r4.
	stmia r4!, {r5-r8}
 
	// If we are still below bss_end, loop.
2:
	cmp r4, r9
	blo 1b

 	// Create page table
	create_page_table
	// Enable MMU
	enable_mmu

	// Call kernel_main
	ldr r3, =kernel_main
	blx r3
 
	// halt
halt:
	wfe
	b halt

// Vector table
.align 4
.globl vec_table
vec_table:
ldr pc,add_handler_00
ldr pc,add_handler_04
ldr pc,add_handler_08
ldr pc,add_handler_0C
ldr pc,add_handler_10
ldr pc,add_handler_14
ldr pc,add_handler_18
ldr pc,add_handler_1C
add_handler_00: .word _start
add_handler_04: .word handler_04
add_handler_08: .word handler_08
add_handler_0C: .word handler_0C
add_handler_10: .word handler_10
add_handler_14: .word handler_14
add_handler_18: .word handler_18
add_handler_1C: .word handler_1C

// Low-level vector table handlers
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

und_stack: .word 0xc0026000
dab_stack: .word 0xc0028000
svc_stack: .word 0xc0032000



