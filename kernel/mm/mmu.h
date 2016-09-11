#ifndef MMU_H
#define MMU_H

#include <stddef.h>
#include <stdint.h>

#define SCTLR_M (1 << 0)

class mmu_device {
public:
	/* System control register */
	uint32_t get_sctlr(void);
	void set_sctlr(uint32_t reg);

	/* Translation base control register */
	uint32_t get_ttbcr(void);
	void set_ttbcr(uint32_t reg);

	/* Translation table base register 0 */
	uint32_t get_ttbr0(void);
	void set_ttbr0(uint32_t reg);

	/* Translation table base register 1 */
	uint32_t get_ttbr1(void);
	void set_ttbr1(uint32_t reg);

	/* Domain access control register */
	uint32_t get_dacr(void);
	void set_dacr(uint32_t reg);

	/* TLB flushing */
	/* Invalidate all entries */
	void tlb_flush_all();

	/* Barriers */
	void isb(void);
	void dsb(void);
private:
};

#endif
