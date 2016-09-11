#include "mm.h"
#include "../common/kernel.h"
#include "../common/lib.h"

extern kernel kern;

void memory_manager::init()
{
	kpgd = (pte_t *) 0x20000;
	/* Map low memory */
	section_set_region(KERNBASE, 0, LOWMEM_RAM, AP_KO | MAP_CACHE | MAP_BUF);
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
