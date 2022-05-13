#include <mm/paging.h>

inline void slice_linear_addr(void *address, short *pde, short *pte, short *offset)
{
	*pde = (int) address >> 22;
	*pte = ((int) address >> 12) & 0x3ff;
	*offset = (int) address & 0xfff;
}

inline void enable_paging(struct page_directory_entry page_dir)
{
	asm ("mov %0, %%eax; mov %%eax, %%cr3; mov %%cr0, %%eax; or $0x80000000, %%eax; mov %%eax, %%cr0": : "m" (page_dir) : "eax" );
}
