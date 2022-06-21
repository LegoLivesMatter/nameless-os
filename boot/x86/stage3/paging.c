/* Code for enabling paging */

#include <stdint.h>

#define ADDRESS_NOT_ALIGNED 1
#define INVL_ADDRESS 2
#define ADDRESS_ALREADY_MAPPED 3
#define ADDRESS_RANGE_MISMATCHED 4

static uint32_t page_directory[1024] __attribute__((aligned(4096))) __attribute__((section(".data")));
static uint32_t page_table_firstmb[1024] __attribute__((aligned(4096))) __attribute__((section(".data")));
static uint32_t page_table_kernel[1024] __attribute__((aligned(4096))) __attribute__((section(".data")));

void enable_paging()
{
	int cr3;
	asm ("mov %0, %%cr3": : "a" (page_directory));
	asm ("mov %%cr0, %0": "=a" (cr3));
	cr3 |= 0x80000000;
	asm ("mov %0, %%cr0": : "a" (cr3));
}

int map_address(void *physical, void *virtual, int flags)
{
	if ((uint32_t) physical & 0xfff || (uint32_t) virtual & 0xfff)
		return ADDRESS_NOT_ALIGNED;

	uint32_t pdir_index = (uint32_t) virtual >> 22;
	if (pdir_index != 0 && pdir_index != 0x300)
		return INVL_ADDRESS;

	uint32_t ptbl_index = ((uint32_t) virtual >> 12) & 0x3ff;
	switch (pdir_index) {
	case 0:
		if (page_table_firstmb[ptbl_index] & 1 == 1)
			return ADDRESS_ALREADY_MAPPED;
		page_table_firstmb[ptbl_index] = ((uint32_t) physical & ~0xfff) | (flags & 0xfff) | 1;
		break;
	case 0x300:
		if (page_table_kernel[ptbl_index] & 1 == 1)
			return ADDRESS_ALREADY_MAPPED;
		page_table_kernel[ptbl_index] = ((uint32_t) physical & ~0xfff) | (flags & 0xfff) | 1;
	}

	return 0;
}

int map_range(void *phys_start, void *phys_end, void *virt_start, void *virt_end, int flags)
{
	if ((uint32_t) phys_start & 0xfff || (uint32_t) phys_end & 0xfff ||
			(uint32_t) virt_start & 0xfff || (uint32_t) virt_end & 0xfff)
		return ADDRESS_NOT_ALIGNED;

	if ((uint32_t) phys_end - (uint32_t) phys_start != (uint32_t) virt_end - (uint32_t) virt_start)
		return ADDRESS_RANGE_MISMATCHED;

	int loops = ((uint32_t) phys_end - (uint32_t) phys_start) / 4096;
	for (int i=0; i<loops; i++) {
		int ret = map_address(phys_start + i*4096, virt_start + i*4096, flags);
		if (ret)
			return ret;
	}
	return 0;
}

void set_up_page_directory()
{
	page_directory[0] = (uint32_t) page_table_firstmb | 3;
	page_directory[0x300] = (uint32_t) page_table_kernel | 3;
}
