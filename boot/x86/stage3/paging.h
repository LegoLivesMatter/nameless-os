#ifndef BOOT_X86_PAGING_H
#define BOOT_X86_PAGING_H

#define ADDRESS_NOT_ALIGNED 1
#define INVL_ADDRESS 2
#define ADDRESS_ALREADY_MAPPED 3
#define ADDRESS_RANGE_MISMATCHED 4

extern int map_range(void *phys_start, void *phys_end, void *virt_start, void *virt_end, int flags);
extern void set_up_page_directory();
extern void enable_paging();

#endif
