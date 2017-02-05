#include "mmu.h"

/* TODO: Remove duplication */

void mmu_device::isb(void)
{
	uint32_t reg = 0;
	asm volatile("mcr p15, 0, %[r], c7, c5, 4": :[r]"r" (reg):);
}

void mmu_device::dsb(void)
{
	uint32_t reg = 0;
	asm volatile("mcr p15, 0, %[r], c7, c10, 4": :[r]"r" (reg):);
}

uint32_t mmu_device::get_sctlr(void)
{
	uint32_t reg;
	asm volatile("mrc p15, 0, %[r], c1, c0, 0": [r]"=r" (reg)::);
	return reg;
};

void mmu_device::set_sctlr(uint32_t reg)
{
	asm volatile("mcr p15, 0, %[r], c1, c0, 0": :[r]"r" (reg):);
};

uint32_t mmu_device::get_ttbcr(void)
{
	uint32_t reg;
	asm volatile("mrc p15, 0, %[r], c2, c0, 2": [r]"=r" (reg)::);
	return reg;
};

void mmu_device::set_ttbcr(uint32_t reg)
{
	asm volatile("mcr p15, 0, %[r], c2, c0, 2": :[r]"r" (reg):);
};

uint32_t mmu_device::get_ttbr0(void)
{
	uint32_t reg;
	asm volatile("mrc p15, 0, %[r], c2, c0, 0": [r]"=r" (reg)::);
	return reg;
};

void mmu_device::set_ttbr0(uint32_t reg)
{
	asm volatile("mcr p15, 0, %[r], c2, c0, 0": :[r]"r" (reg):);
};

uint32_t mmu_device::get_ttbr1(void)
{
	uint32_t reg;
	asm volatile("mrc p15, 0, %[r], c2, c0, 1": [r]"=r" (reg)::);
	return reg;
};

void mmu_device::set_ttbr1(uint32_t reg)
{
	asm volatile("mcr p15, 0, %[r], c2, c0, 1": :[r]"r" (reg):);
};

uint32_t mmu_device::get_dacr(void)
{
	uint32_t reg;
	asm volatile("mrc p15, 0, %[r], c3, c0, 0": [r]"=r" (reg)::);
	return reg;
};

void mmu_device::set_dacr(uint32_t reg)
{
	asm volatile("mcr p15, 0, %[r], c3, c0, 0": :[r]"r" (reg):);
};

void mmu_device::tlb_flush_all(void)
{
	uint32_t reg = 0;
	asm volatile("mcr p15, 0, %[r], c8, c7, 0" : :[r]"r" (reg):);
};

void mmu_device::tlb_flush_va(uint32_t reg)
{
	reg &= (~0xFFF);
	/* TLB invalidate U MVA (shareable) */
	asm volatile("mcr p15, 0, %[r], c8, c3, 1": :[r]"r" (reg):);
	/* TLB invalidate U MVA */
	asm volatile("mcr p15, 0, %[r], c8, c7, 1": :[r]"r" (reg):);
};
