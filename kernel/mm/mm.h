#ifndef MM_H
#define MM_H
#include <stddef.h>
#include <stdint.h>
#include "../common/lib.h"
#include "mmu.h"

/* Page table entry represent section mapping
 * i.e. 1MB section*/
#define SECTION_TYPE	0x2
/* Page table entry represent page table mapping
 * i.e. 1MB is described by this page table*/
#define PAGE_TABLE_TYPE	0x1
/* Page table entry represents second level page
 * table base address*/
#define SMALL_PAGE_TYPE	0x2
#define LARGE_PAGE_TYPE	0x1

/* Cacheable mapping */
#define MAP_CACHE	(1 << 3)
/* Bufferable mapping */
#define MAP_BUF		(1 << 2)

/* Access permission for page directory/page table entries. */
/* AP[2:0] Model */
/* For sections */
#define AP0_SEC		(1 << 10)
#define AP1_SEC		(1 << 11)
#define AP2_SEC		(1 << 15)
#define AP_KRW_SEC	(AP0_SEC)
/* For small pages short desc format  B3-1358*/
#define AP0_SMP		(1 << 4)
#define AP1_SMP		(1 << 5)
#define AP2_SMP		(1 << 9)
#define AP_KRW_SMP	(AP0_SMP)
#define AP_KRWURW_SMP	(AP0_SMP | AP1_SMP)

#define PDE_ADDR(desc)  (desc & (~0x3))
#define PTE_ADDR(desc)  (desc & (~0xFFF))

typedef uint32_t pde_t;
typedef uint32_t pte_t;
typedef uint32_t physaddr_t;
typedef void * virtaddr_t;


#define TTBCR_N_MASK	0x7

#define PAGE_SIZE	4096
#define PAGE_SHIFT	12

#define KERNBASE	0xC0000000
#define KPGD_BASE	0x20000
#define KPGD_VIRT	(KPGD_BASE + KERNBASE)
#define KPGD_END	0x24000

#define RAM_OVERALL	PBASE
#define PERIPH_SIZE	0x01000000

#define UTOP		0x40000000
#define USTACKTOP	UTOP
extern char __end[];

/*          Boot memory MAP
 *================================
 *
 *
 *--------------------------------
 *
 *		page_list
 *
 *--------------------------------__end[]
 *
 *
 *      Binary sections (.text, ...)
 *-------------------------------- 0xc0032000
 *
 *	SVC mode stack
 *-------------------------------- 0xc0028000
 *
 *	Data abort stack
 *-------------------------------- 0xc0026000
 *
 *	Undefined mode stack
 *-------------------------------- 0xc0024000
 *
 *   Kernel page directory, lvl 1
 *-------------------------------- 0xc0020000
 *
 *		KERNBASE
 *-------------------------------- 0xc0000000 */

/*
Translation flow for a Small page  (short desc format)
Figure shows the complete translation flow for a Small page.

 31		 20               12
 +---------------+-----------------+--------------------+
 |   L1 index    |    L2 index     |  Page offset       |
 +------------------------------------------------------+

For more info see B3 Virtual Memory System Architecture (VMSA)
B3.5 Short-descriptor translation table format
*/
#define L2SHIFT 12
#define L1SHIFT 20

#define L1IDX(la)	((((uintptr_t) (la)) >> L1SHIFT) & 0xFFF)

#define L2IDX(la)	((((uintptr_t) (la)) >> L2SHIFT) & 0xFF)

#define PGOFF(la)	(((uintptr_t) (la)) & 0xFFF)

#define PGNUM(la)	(((uintptr_t) (la)) & 0xFFFFF000)

typedef uint32_t alloc_mask;
#define ALLOC_ZERO 0x1

struct page {
	/* Next page */
	struct page *p_next;
	/* Reference counter */
	uint16_t ref_cnt;
};

class page_allocator {
private:
	char *boot_nextfree;
public:
	static struct page *page_arr;
	static struct page *free_page_list;

	void *boot_alloc(uint32_t n);
	void mem_init();
	struct page *alloc_page(alloc_mask mask);
	void free_page(struct page *page);

	page_allocator() : boot_nextfree((char *)round_up((uint32_t)__end, PAGE_SIZE)) {};
};

class memory_manager {
private:
	/* MMU device */
	pde_t *kpgd;
public:
	mmu_device mmu;
	page_allocator palloc;
	memory_manager() : kpgd((pde_t *)KPGD_VIRT) {};
	void early_vm_map();
	void section_set_region(uintptr_t va, uintptr_t pa, size_t len, uint32_t attrs);
	pte_t *get_pte(pde_t *pgdir, const void *va, int create);
	int page_insert(pde_t *pgdir, struct page *p, void *va, int perm);
	struct page *page_lookup(pde_t *pgdir, void *va);
	void page_remove(pde_t *pgdir, void *va);
	void region_alloc(pde_t *pgdir, void *va, size_t len);
};

static inline physaddr_t virt_to_phys(uint32_t virt_addr)
{
	return virt_addr - KERNBASE;
}

static inline uint32_t phys_to_virt(physaddr_t phys_addr)
{
	return phys_addr + KERNBASE;
}

static inline physaddr_t page_to_phys(struct page *page)
{
	return (page - page_allocator::page_arr) << PAGE_SHIFT;
}

static inline struct page *phys_to_page(physaddr_t phys_addr)
{
	return &page_allocator::page_arr[phys_addr >> PAGE_SHIFT];
}

static inline uint32_t page_to_virt(struct page *page)
{
	return phys_to_virt(page_to_phys(page));
}


#endif
