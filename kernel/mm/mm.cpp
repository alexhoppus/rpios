#include "mm.h"
#include "../common/kernel.h"
#include "../common/lib.h"

struct page *page_allocator::page_list = NULL;
struct page *page_allocator::free_page_list = NULL;

void memory_manager::early_vm_map()
{
	kpgd = (pte_t *) KPGD_BASE;
	/* Map low memory */
	section_set_region(KERNBASE, 0, RAM_OVERALL, AP_KO | MAP_CACHE | MAP_BUF);
	/* Map periph. */
	section_set_region(VPBASE, PBASE, PERIPH_SIZE, AP_KO);
}

void memory_manager::section_set_region(uintptr_t va, uintptr_t pa, size_t len, uint32_t attrs)
{
	va >>= PDE_SHIFT;
	pa >>= PDE_SHIFT;
	len >>= PDE_SHIFT;

	for (int i = 0; i < (int)len; i++, va++, pa++) {
		pde_t pde = (pa << PDE_SHIFT) | SECTION_TYPE | attrs;
		kpgd[va] = pde;
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

void page_allocator::init_page_list() {
	int pages_available = 0;
	int n_pages = PBASE / PAGE_SIZE;
	page_list = (struct page *)boot_alloc(n_pages * sizeof(struct page));
	if (!page_list)
		panic("Out of memory can't init page_list\n");

	for (int i = 0; i < n_pages; i++) {
		uint32_t phys_addr = i << PAGE_SHIFT;
		if (phys_to_virt(phys_addr) > round_up((uint32_t)boot_nextfree, PAGE_SIZE)) {
			page_list[i].p_next = free_page_list;
			free_page_list = &page_list[i];
			pages_available++;
		}
	}
	kern.cons->cprintf("n_pages:         %d\n", n_pages);
	kern.cons->cprintf("pages_available: %d\n", pages_available);
}

struct page *page_allocator::alloc_page(alloc_mask mask)
{
	struct page *next_free_page = free_page_list;
	if (!next_free_page)
		return NULL;
	free_page_list = next_free_page->p_next;
	if (mask & ALLOC_ZERO)
		memset((void *)page_to_virt(next_free_page), 0, PAGE_SIZE);
	return next_free_page;
}

void page_allocator::free_page(struct page *page)
{
	if (--page->ref_cnt == 0) {
		page->p_next = free_page_list;
		free_page_list = page;
	}
	bug_on(page->ref_cnt < 0);
}
