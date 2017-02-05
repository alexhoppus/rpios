#include "mm.h"
#include "kernel.h"
#include "lib.h"
#include "sched.h"

struct page *page_allocator::page_arr = NULL;
struct page *page_allocator::free_page_list = NULL;

pte_t *memory_manager::get_pte(pde_t *pgdir, const void *va, int create)
{
	struct page *p;
	dprintk("va: %lx create %d pgdir %lx\n", va, create, pgdir);
	if (va > (void *)KERNBASE)
		return NULL;
	pde_t *pddir = &pgdir[L1IDX(va)];
	dprintk("*pddir: %lx\n", *pddir);
	if (!(*pddir)) {
		/* L1 page table doesn't exist*/
		if (!create) {
			return NULL;
		} else {
			p = palloc.alloc_page(ALLOC_ZERO);
			if (!p)
				return NULL;
			physaddr_t pa = page_to_phys(p);
			*pddir = (pa | PAGE_TABLE_TYPE);
			dprintk(" -> *pddir: %lx\n", *pddir);
			p->ref_cnt++;
		}
	}
	pte_t *pddir_va = (pte_t *)(PDE_ADDR(*pddir) + KERNBASE);
	return &(pddir_va[L2IDX(va)]);
}

struct page *memory_manager::page_lookup(pde_t *pgdir, void *va)
{
	dprintk("va: %lx\n", va);
	pte_t *pte = get_pte(pgdir, va, 0);
	if (!(*pte))
		return NULL;
	return phys_to_page(PTE_ADDR(*pte));
}

void memory_manager::page_remove(pde_t *pgdir, void *va)
{
	pte_t *pte;
	dprintk("va: %lx\n", va);
	struct page *p = page_lookup(pgdir, va);
	if (!p)
		return;
	palloc.free_page(p);
	pte = get_pte(pgdir, va, 0);
	*pte = 0;
	mmu.tlb_flush_va((uint32_t)va);
}

int memory_manager::page_insert(pde_t *pgdir, struct page *p, void *va, int perm)
{
	dprintk("va: %lx pa: %lx\n", va, page_to_phys(p));
	pte_t *pte = get_pte(pgdir, va, 1);
	if (!pte)
		return -1;
	dprintk("*pte %lx\n", (int32_t) *pte);
	if (*pte && page_to_phys(p) != PTE_ADDR(*pte)) {
		page_remove(pgdir, va);
		mmu.tlb_flush_va((uint32_t)va);
	}
	if (page_to_phys(p) != PTE_ADDR(*pte))
		p->ref_cnt++;
	physaddr_t pa = page_to_phys(p);
	*pte = (pa | (perm | SMALL_PAGE_TYPE));
	return 0;
}

void memory_manager::early_vm_map()
{
	uint32_t ttbcr;
	/* Do no't spoil first megabyte identity mapping */
	memset((void *)(kpgd), 0x0, PAGE_SIZE * 4);
	/* Set TTBCR to use both TTBR0 and TTBR1 registers */
	ttbcr = mmu.get_ttbcr();
	ttbcr &= (~TTBCR_N_MASK);
	/* Set split of TTBR0/1 to 0x40000000 */
	ttbcr |= 0x02;
	mmu.set_ttbcr(ttbcr);
	mmu.set_ttbr0(KPGD_BASE);
	mmu.set_ttbr1(KPGD_BASE + PAGE_SIZE);

	/* Map low memory */
	section_set_region(KERNBASE, 0, RAM_OVERALL, AP_KRW_SEC | MAP_CACHE | MAP_BUF);
	/* Map periph. */
	section_set_region(VPBASE, PBASE, PERIPH_SIZE, AP_KRW_SEC);
	mmu.isb();
	mmu.tlb_flush_all();
}

void memory_manager::section_set_region(uintptr_t va, uintptr_t pa, size_t len, uint32_t attrs)
{
	va >>= L1SHIFT;
	pa >>= L1SHIFT;
	len >>= L1SHIFT;

	for (int i = 0; i < (int)len; i++, va++, pa++) {
		pde_t pde = (pa << L1SHIFT) | SECTION_TYPE | attrs;
		kpgd[va] = pde;
	}
}

void memory_manager::region_alloc(pde_t *pgdir, void *va, size_t len)
{
	int n_alloc_pages = 0;
	uint32_t beg_va = ((uint32_t)va & (~((1 << PAGE_SHIFT) - 1)));
	uint32_t end_va = (((uint32_t)((size_t)va + len) - 1) + PAGE_SIZE) &
		(~((1 << PAGE_SHIFT) - 1));
	dprintk("beg_va %lx end_va %lx\n", beg_va, end_va);
	n_alloc_pages = (end_va - beg_va) / PAGE_SIZE;
	for (int i = 0; i < n_alloc_pages; i++) {
		struct page *p;
		if (!(p = palloc.alloc_page(ALLOC_ZERO))) {
			panic("region alloc failed\n");
		}
		page_insert(pgdir, p, (void *)(beg_va + i * PAGE_SIZE), AP_KRWURW_SMP | MAP_CACHE | MAP_BUF);
	}

}

void *page_allocator::boot_alloc(uint32_t n)
{
	if (!boot_nextfree)
		boot_nextfree = (char *)round_up((uint32_t) __end, PAGE_SIZE);
	if (n < 0)
		panic("Cant allocate negative quantity\n");
	if (n == 0)
		return boot_nextfree;
	char *saved_nextfree = boot_nextfree;
	boot_nextfree += PAGE_SIZE * (((n - 1) >> PAGE_SHIFT) + 1);
	if ((uint32_t) boot_nextfree > KERNBASE + RAM_OVERALL)
		panic("boot_alloc: Out of memory");
	return (void *)saved_nextfree;
}

void page_allocator::mem_init()
{
	int pages_available = 0;
	int n_pages = RAM_OVERALL / PAGE_SIZE;
	page_arr = (struct page *)boot_alloc(n_pages * sizeof(struct page));
	if (!page_arr)
		panic("Out of memory can't init page_arr\n");
	memset(page_arr, 0x0, n_pages * sizeof(struct page));

	scheduler::task_arr = (struct task *)boot_alloc(N_MAX_TSK * sizeof(struct task));
	if (!scheduler::task_arr)
		panic("Out of memory can't init task_list\n");
	memset(scheduler::task_arr, 0x0, N_MAX_TSK * sizeof(struct task));

	for (int i = 0; i < n_pages; i++) {
		uint32_t phys_addr = i << PAGE_SHIFT;
		if (phys_to_virt(phys_addr) > round_up((uint32_t)boot_nextfree, PAGE_SIZE)) {
			page_arr[i].p_next = free_page_list;
			free_page_list = &page_arr[i];
			pages_available++;
		}
	}
	printk("n_pages:         %d\n", n_pages);
	printk("pages_available: %d\n", pages_available);
}

struct page *page_allocator::alloc_page(alloc_mask mask)
{
	struct page *next_free_page = free_page_list;
	if (!next_free_page)
		return NULL;
	free_page_list = next_free_page->p_next;
	if (mask & ALLOC_ZERO)
		memset((void *)page_to_virt(next_free_page), 0, PAGE_SIZE);
	dprintk("page #%lx allocated\n", page_to_phys(next_free_page) >> PAGE_SHIFT);
	return next_free_page;
}

void page_allocator::free_page(struct page *page)
{
	dprintk("page #%lx free ref_cnt %d\n", page_to_phys(page) >> PAGE_SHIFT, page->ref_cnt);
	if (--page->ref_cnt == 0) {
		page->p_next = free_page_list;
		free_page_list = page;
	}
	bug_on(page->ref_cnt < 0);
}

memory_manager mm;
