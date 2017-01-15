#include "exception.h"
#include "../sched/sched.h"
#include "../common/kernel.h"

struct desc {
	uint32_t num;
	char desc[16];
};

struct desc md [] = {
	{MODE_USR, "USR"},
	{MODE_FIQ, "FIQ"},
	{MODE_IRQ, "IRQ"},
	{MODE_SVC, "SVC"},
	{MODE_MON, "MON"},
	{MODE_ABT, "ABT"},
	{MODE_HYP, "HYP"},
	{MODE_UND, "UND"},
};

static char *mode_name(uint32_t mode)
{
	for (int i = 0; i < (int) sizeof(md); i++) {
		if (md[i].num == mode)
			return md[i].desc;
	}
	return (char *)"null";
}

extern "C" void abort_handler(struct trapframe *tf)
{
	printk("Exception %s occured at address %lx in %s mode\n", mode_name(get_cpsr() & MODE_MASK), tf->pc, mode_name(tf->spsr & MODE_MASK));
	printk("Trapframe:\n");
	printk("sp_usr %lx\n", tf->sp_usr);
	printk("lr_usr %lx\n", tf->lr_usr);
	printk("sp_svc %lx\n", tf->sp_svc);
	printk("lr_svc %lx\n", tf->lr_svc);
	printk("spsr %lx\n", tf->spsr);
	for (int i = 0; i < N_GEN_REGS; i++) {
		printk("r[%d] = %lx\n", i, tf->r[i]);
	}
	while(1){};
}

int sys_get_pid(int a1, int a2, int a3, int a4)
{
	return sc.current->pid;
}

int sys_puts(int a1, int a2, int a3, int a4)
{
	printk("%s\n", (char *)a1);
	return 0;
}

int sys_yield(int a1, int a2, int a3, int a4)
{
	sc.resched();
	return 0;
}

int (*syscall_array[4])(int, int, int, int) = {
	NULL,
	sys_get_pid,
	sys_puts,
	sys_yield
};

extern "C" void swi_handler(struct trapframe *tf)
{
	int num = tf->r[0], ret;
	dprintk("syscall %d task %d\n", num, sc.current->pid);
	ret = syscall_array[tf->r[0]](tf->r[1], tf->r[2], tf->r[3], tf->r[4]);
	tf->r[0] = ret;
	return;
}

extern uint32_t vec_table;
void install_vector_table()
{
	asm volatile("mcr p15, 0, %[r], c12, c0, 0": :[r]"r" (&vec_table):);
}




