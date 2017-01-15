#include "../common/lib.h"
#include "../mm/mm.h"
#include "../common/kernel.h"
#include "debug.h"

#if DEBUG
void check_mm()
{
	uint32_t addr = 0xb0000000;
	struct page *p = mm.palloc.alloc_page(ALLOC_ZERO);
	int *vaddr1 = (int *)page_to_virt(p);
	if (vaddr1[1] != 0)
		panic("failed to alloc page with ALLOC_ZERO\n");
	vaddr1[0] = 1; vaddr1[1] = 2; vaddr1[3] = 3;
	mm.page_insert((pde_t *)KPGD_VIRT, p, (void *)addr, (AP_KRW_SMP | MAP_CACHE | MAP_BUF));
	int *vaddr2 = (int *)addr;
	if (vaddr2[0] != 1 || vaddr2[1] != 2 || vaddr2[3] != 3)
		panic("Invalid values after page mapping\n");
	mm.page_remove((pde_t *)KPGD_VIRT, (void *)addr);
	struct page *p2 = mm.page_lookup((pde_t *)KPGD_VIRT, (void *)addr);
	if (p2)
		panic("Page was not removed\n");
};
#else
void check_mm() {};
#endif

