#ifndef EH_H
#define EH_H
#include <stddef.h>
#include <stdint.h>

void install_vector_table(void);

extern "C" void trapret(void);

#define I_BIT (1 << 7)
#define F_BIT (1 << 6)

static inline void set_cpsr(uint32_t reg)
{
	asm volatile("msr cpsr, %[r]": :[r]"r" (reg):);
}

static inline uint32_t get_cpsr(void)
{
	uint32_t reg;
	asm volatile("mrs %[r], cpsr": [r]"=r" (reg)::);
	return reg;
}

static inline void set_spsr(uint32_t reg)
{
	asm volatile("msr spsr, %[r]": :[r]"r" (reg):);
}

static inline uint32_t get_spsr(void)
{
	uint32_t reg;
	asm volatile("mrs %[r], spsr": [r]"=r" (reg)::);
	return reg;
}

static inline void irq_disable()
{
	set_cpsr(get_cpsr() | I_BIT);
}

static inline void irq_enable()
{
	set_cpsr(get_cpsr() & (~I_BIT));
}

#define N_GEN_REGS 13

struct trapframe {
	uint32_t   sp_usr;     // user mode sp
	uint32_t   lr_usr;     // user mode lr
	uint32_t   sp_svc;
	uint32_t   lr_svc;
	uint32_t   spsr;
	uint32_t   r[N_GEN_REGS];
	uint32_t   pc;         // (lr on entry) instruction to resume execution
};

#endif
