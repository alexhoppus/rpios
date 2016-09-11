#ifndef MM_H
#define MM_H
#include <stddef.h>
#include <stdint.h>

/* Page table entry represent section mapping
 * i.e. 1MB section*/
#define SECTION_TYPE	0x2
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
#define PGSHIFT		12

#define KERNBASE	0xC0000000
#define KPGD_BASE	0x20000

#define INITMAP		(1 << 20)

#define RAM_OVERALL 0x40000000
#define LOWMEM_RAM  0x10000000
#define PERIPH_SIZE 0x01000000
extern char __end[];

struct page {
	/* Reference counter */
	uint16_t ref_cnt;
};

// TODO: List of pages
class page_allocator {

};

class memory_manager {
private:
	pde_t *kpgd;
public:
	page_allocator palloc;
	memory_manager() {};
	void init();
	void section_set_region(uintptr_t va, uintptr_t pa, size_t len, uint32_t attrs);
};
#endif
