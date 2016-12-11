#include "uart/uart.h"
#include "common/stubs.h"
#include "common/lib.h"
#include "mm/mm.h"
#include "common/kernel.h"
#include "sched/sched.h"
#include "debug/debug.h"

typedef void constructor_func(void);
extern char __init_array_start[];
extern char __init_array_end[];

inline void global_constructors_init()
{
	for (uint32_t *p = (uint32_t *)__init_array_start; p < (uint32_t *)(__init_array_end); ++p) {
		constructor_func *cf = (constructor_func *)(*p);
		cf();
	}
}

#if defined(__cplusplus)
extern "C"
#endif
void exception_handler(uint32_t r0)
{
	uint32_t lr;
	asm volatile("mov %[r], lr": [r]"=r" (lr)::);;
	printk("Exception with code %x occured at address %x\n", r0, lr);
	while(1){};
}

extern uint32_t vec_table;
void install_vector_table()
{
	asm volatile("mcr p15, 0, %[r], c12, c0, 0": :[r]"r" (&vec_table):);
	kern.mmu.isb();
}

extern char _binary_app_start[];

#if defined(__cplusplus)
extern "C"
#endif
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
	(void) r0;
	(void) r1;
	(void) atags;
	global_constructors_init();
	/* Lowmem mapping */
	mm.early_vm_map();
	/* Setup UART as main console */
	cons = &uart;
	cons->init();

	// TODO: Think about exception handling class
	install_vector_table();


	mm.palloc.init_page_list();
	check_mm();

	sc.init_task_list();
	task *t1 = sc.alloc_task();
	t1->init_vm();
	t1->load_binary(_binary_app1_start);

	task *t2 = sc.alloc_task();
	t2->init_vm();
	t2->load_binary(_binary_app2_start);

	sc.run();
	while(1) {};
}
