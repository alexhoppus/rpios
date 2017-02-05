// Vector table
#define DAB_MODE  0x17
#define UND_MODE  0x1B
#define HYP_MODE  0x1A
#define SVC_MODE  0x13
#define IRQ_MODE  0x12
#define FIQ_MODE  0x11

#define MODE_MASK 0x1f

#define I_BIT (1 << 7)
#define F_BIT (1 << 6)

.global trapret

.align 5
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
add_handler_04: .word handler_und
add_handler_08: .word handler_swi
add_handler_0C: .word handler_iab
add_handler_10: .word handler_dab
add_handler_14: .word 0xdeaddead
add_handler_18: .word handler_irq
add_handler_1C: .word handler_fiq

/* Trapframe format

struct trapframe {
	uint32_t   sp_usr;     // user mode sp
	uint32_t   lr_usr;     // user mode lr
	uint32_t   sp_svc;
	uint32_t   lr_svc;
	uint32_t   spsr;
	uint32_t   r[N_GEN_REGS];
	uint32_t   pc;         // (lr on entry) instruction to resume execution
};
*/
/* Build trapframe in exception stack 
   r0 - trapframe pointer
*/
.macro build_trapframe
	push {lr}
	push {r0-r12}
	mrs r0, spsr
	push {r0}

	mrs r0, cpsr
	mvn r1, #MODE_MASK
	and r2, r0, r1
	orr r2, r2, #SVC_MODE 
	msr cpsr, r2
	mov r3, r13
	mov r4, r14
	msr cpsr, r0

	push {r3, r4}

	stmfd r13, {sp, lr}^
	sub r13, r13, #8
	mov r0, r13
.endm

handler_und:
sub r14, r14, #4
build_trapframe
bl abort_handler

handler_iab:
sub r14, r14, #4
build_trapframe
bl abort_handler

handler_dab:
sub r14, r14, #8
build_trapframe
bl abort_handler

handler_irq:
    sub    r14, r14, #4            // r14 (lr) contains the interrupted PC
    push   {r0-r2, r14}
    mrs    r1, spsr                // save spsr_irq
    mov    r0, r13                 // save irq stack
    add    r13, r13, #16           // reset the IRQ stack

    # switch to the SVC mode
    mrs     r2, cpsr
    bic     r2, r2, #MODE_MASK
    orr     r2, r2, #SVC_MODE | I_BIT
    msr     cpsr_cxsf, r2

    # now, in SVC mode, sp, lr, pc (r13, r14, r15) are all banked 
    # build the trap frame
    ldr     r2, [r0, #12]           // read the r14_irq, then save it
    push    {r2}
    push    {r3-r12}                // r4-r12 are preserved (non-banked)
    ldmfd   r0, {r3-r5}             // copy r0-r2 over from irq stack
    push    {r3-r5}
    push    {r1}
    push    {lr}
    sub     r13, r13, #4

    stmfd   r13, {sp, lr}^          // save user mode sp and lr
    sub     r13, r13, #8

    # get the parameters, then call the handler
    mov     r0, r13                 // points to
    bl      irq_handler

    # restore the previous status
    b       trapret

handler_fiq:
b handler_fiq

handler_swi:
build_trapframe
bl swi_handler

trapret:
    ldmfd   r13, {sp, lr}^      // restore user mode sp and lr
    add     r13, r13, #8
    ldmfd   r13!, {r14}         // restore r14
    add	    r13, r13, #4
    ldmfd   r13!, {r2}         // restore spsr
    msr     spsr_cxsf, r2
    ldmfd   r13!,{r0-r12, pc}^  // restore context and return
