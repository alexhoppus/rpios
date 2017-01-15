#include "uart/uart.h"
#include "common/stubs.h"
#include "common/lib.h"
#include "mm/mm.h"
#include "common/kernel.h"
#include "sched/sched.h"
#include "debug/debug.h"
#include "exception/exception.h"

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

extern char _binary_app1_start[];
extern char _binary_app2_start[];

extern "C" void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
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

	install_vector_table();

	mm.palloc.mem_init();
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
