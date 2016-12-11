#ifndef SCHED_H
#define SCHED_H
#include <stddef.h>
#include <stdint.h>
#include "../common/lib.h"
#include "../mm/mm.h"
#include "../exception/exception.h"
#include "elf.h"

#define N_MAX_TSK	100

#define MODE_MASK	0x1f

#define MODE_USR	0x10
#define MODE_FIQ	0x11
#define MODE_IRQ	0x12
#define MODE_SVC	0x13
#define MODE_MON	0x16
#define MODE_ABT	0x17
#define MODE_HYP	0x1a
#define MODE_UND	0x1b
#define MODE_SYS	0x1f

enum tstate {TUNUSED, TRUNNABLE, TRUNNING};

struct context {
	uint32_t    r4;
	uint32_t    r5;
	uint32_t    r6;
	uint32_t    r7;
	uint32_t    r8;
	uint32_t    r9;
	uint32_t    r10;
	uint32_t    r11;
	uint32_t    r12;
	uint32_t    lr;
};

class task {
private:
public:
	uint32_t pid;
	pde_t *pgd;
	tstate state;
	uint32_t *kstack;
	context *ctx;
	trapframe *tf;
	task() {};
	void init_vm();
	int load_binary(char *binary_obj);
};

class scheduler {
private:
	/* Scheduler kthread context */
	struct context *ctx;
	void swtch(struct context **oldctx, struct context *newctx);
public:
	task *current;
	static task *task_arr;
	scheduler() {};
	void init_task_list();
	task *alloc_task();
	void free_task(task *task);
	void run();
	void resched(); /* Give control back to scheduler thread */
};
#endif

