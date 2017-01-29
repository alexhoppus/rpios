#include "sched.h"
#include "elf.h"
#include "../common/kernel.h"
#include "../common/lib.h"

class task *scheduler::task_arr = NULL;


void task::init_vm()
{
	struct page *p = mm.palloc.alloc_page(ALLOC_ZERO);
	pgd = (pde_t *)page_to_virt(p);
	dprintk("task pgd va: %lx\n", (uint32_t)pgd);
}

int task::load_binary(char *binary)
{
	struct elfhdr_layout *elf = (struct elfhdr_layout *) binary;
	struct proghdr_layout *ph, *eph;
	dprintk("binary %lx\n", binary);
	if (elf->e_magic != ELF_MAGIC)
		panic("ELF not found in load_binary\n");

	mm.mmu.set_ttbr0(virt_to_phys((uint32_t)pgd));
	mm.mmu.tlb_flush_all();
	ph = (struct proghdr_layout *) (binary + elf->e_phoff);
	eph = ph + elf->e_phnum;
	for (; ph < eph; ph++) {
		if (ph->p_type == ELF_PROG_LOAD) {
			mm.region_alloc(pgd, (void *)ph->p_va, ph->p_memsz);
			memcpy((void *)ph->p_va, binary + ph->p_offset, ph->p_filesz);
		}
	}
	mm.region_alloc(pgd, (void *) (USTACKTOP - PAGE_SIZE), PAGE_SIZE);
	struct page *kstack_page = mm.palloc.alloc_page(ALLOC_ZERO);
	if (!kstack_page)
		panic("Not enough memory to allocate kernel stack page\n");
	kstack = (uint32_t *)page_to_virt(kstack_page);
	char *sp = (char *)kstack + PAGE_SIZE;
	sp -= sizeof(struct trapframe);
	tf = (struct trapframe *) sp;
	memset(tf, 0x0, sizeof(struct trapframe));
	tf->spsr = MODE_USR | F_BIT;
	tf->sp_usr = USTACKTOP;
	tf->pc = elf->e_entry;

	sp -= sizeof (struct context);
	ctx = (struct context *) sp;
	memset(ctx, 0x0, sizeof(struct context));
	ctx->lr = (uint32_t) trapret;

	dprintk("kstack top: %lx\n", (char *) kstack + PAGE_SIZE);
	dprintk("ctx: %lx\n", (uint32_t) ctx);
	dprintk("tf: %lx\n", (uint32_t) tf);
	dprintk("trapret: %lx\n", (uint32_t) trapret);
	dprintk("tf->pc: %lx\n", (uint32_t) tf->pc);
	mm.mmu.set_ttbr0(virt_to_phys(KPGD_VIRT));
	mm.mmu.tlb_flush_all();
	state = TRUNNABLE;
	return 0;
}

void scheduler::init_task_list()
{
	for (int i = 0; i < N_MAX_TSK; i++) {
		struct task *t = &scheduler::task_arr[i];
		t->pid = i;
		t->state = TUNUSED;
	}
}

task *scheduler::alloc_task()
{
	for (int i = 0; i < N_MAX_TSK; i++) {
		struct task *t = &scheduler::task_arr[i];
		if (t->state == TUNUSED) {
			dprintk("task %d allocated\n", i);
			return t;
		}
	}
	return NULL;
}

void scheduler::free_task(task *task)
{
	dprintk("task %d freed\n", task->pid);
	task->state = TUNUSED;
}

void scheduler::swtch(struct context **oldctx, struct context *newctx)
{
	/* r0-r3 are not preserved during call, no need to save them */
	asm volatile("push {r4-r12, lr}");
	/* save current kernel thread sp to oldctx */
	asm volatile("str r13, [r1]");
	/* Load newctx (new sp) to sp register */
	asm volatile("mov r13, r2");
	/* Load all other registers from new ctx,
	 *  refer struct context format for details */
	asm volatile("pop {r4-r12, lr}");
}

void scheduler::run()
{
	while(true) {
		for (int i = 0; i < N_MAX_TSK; i++) {
			struct task *t = &scheduler::task_arr[i];
			if (t->state == TRUNNABLE) {
				dprintk("run task %d\n", i);
				mm.mmu.set_ttbr0(virt_to_phys((uint32_t)t->pgd));
				mm.mmu.tlb_flush_all();
				dprintk("swtch (&ctx %lx, t->ctx %lx)\n", &ctx, t->ctx);
				current = t;
				swtch(&ctx, t->ctx);
			}
		}
	}
}


void scheduler::resched()
{
	dprintk("task %d called resched\n", current->pid);
	dprintk("swtch (&t->ctx %lx, ctx %lx)\n", &current->ctx, ctx);
	swtch(&current->ctx, ctx);
}
scheduler sc;
