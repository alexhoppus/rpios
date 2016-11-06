#ifndef MM_H
#define MM_H
#include <stddef.h>
#include <stdint.h>
#include "../common/lib.h"

/* Page table entry represent section mapping
 * i.e. 1MB section*/
#define SECTION_TYPE	0x2
/* Page table entry represent page table mapping
 * i.e. 1MB is described by this page table*/
#define PAGE_TABLE_TYPE	0x1
/* Page table entry represents second level page
 * table base address*/
#define PAGE_TYPE	0x1

/* Cacheable mapping */
#define MAP_CACHE	(1 << 3)
/* Bufferable mapping */
#define MAP_BUF		(1 << 2)

/* Access permission for page directory/page table entries. */
/* No access */
#define AP_NA		((0x00) << 10)
/* Privileged access, kernel: RW, user: no access */
#define AP_KO		((0x01) << 10)
/* No write access from user, read allowed */
#define AP_KUR		((0x02) << 10)
/* Full access */
#define AP_KU		((0x03) << 10)

/* First level page table address shift */
#define PDE_SHIFT	20

typedef uint32_t pde_t;
typedef uint32_t pte_t;

#define PAGE_SIZE	4096
#define PAGE_SHIFT		12

#define KERNBASE	0xC0000000
#define KPGD_BASE	0x20000
#define KPGD_END	0x24000

#define INITMAP		(1 << 20)

#define RAM_OVERALL 0x40000000
#define EARLY_RAM   0x6400000
#define PERIPH_SIZE 0x01000000
extern char __end[];

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
	static struct page *page_list;
	static struct page *free_page_list;

	void *boot_alloc(uint32_t n);
	void init_page_list();
	struct page *alloc_page(alloc_mask mask);
	void free_page(struct page *page);
	page_allocator() : boot_nextfree((char *)round_up((uint32_t)__end, PAGE_SIZE)) {};
};

class memory_manager {
private:
	pde_t *kpgd;
public:
	page_allocator palloc;
	memory_manager() : kpgd((pde_t *)KPGD_BASE) {};
	void early_vm_map();
	void section_set_region(uintptr_t va, uintptr_t pa, size_t len, uint32_t attrs);
};

static inline uint32_t virt_to_phys(uint32_t virt_addr)
{
	return virt_addr - KERNBASE;
}

static inline uint32_t phys_to_virt(uint32_t phys_addr)
{
	return phys_addr + KERNBASE;
}

static inline uint32_t page_to_phys(struct page *page)
{
	return (page - page_allocator::page_list) << PAGE_SHIFT;
}
static inline uint32_t page_to_virt(struct page *page)
{
	return phys_to_virt(page_to_phys(page));
}


#endif
