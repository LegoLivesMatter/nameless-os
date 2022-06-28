#include "elf.h"
#include "paging.h"

/* This bit of code will be run right after the segment registers have been set
 * up for protected mode. */

const char *error_messages[] = {
	"Tried to page an invalid address, the bootloader is buggy!",
	"Tried to page an invalid address, the bootloader is buggy!",
	"Tried to page an invalid address, the bootloader is buggy!",
	"Tried to page an invalid address, the bootloader is buggy!",
	"KERNEL.ELF is not a valid ELF file!",
	"KERNEL.ELF is a non-x86 or userspace executable!",
	"KERNEL.ELF is not an executable ELF file!"
};

void printerr(const char *error)
{
	char *message = (char *) error;
	for (int i=0;;i++) {
		char temp = *message;
		if (!temp) return;
		*(char *) (0xb8000+i*2) = temp;
		*(char *) (0xb8000+1+i*2) = 0xcf;
		message++;
	}
}

void halt()
{
loop:
	asm("hlt":);
	goto loop;
}

void run_kernel(void *e820_map, unsigned int e820_map_size, struct elf_header *kernel, unsigned int kernel_size)
{
	/* Before trying to parse our kernel ELF, let's set up paging. */
	unsigned int bootloader_mem_size = 0x100000 + kernel_size;

	/* Align the size to a 4K boundary. */
	if (bootloader_mem_size & 0xfff) {
		bootloader_mem_size += 0x1000;
		bootloader_mem_size &= ~0xfff;
	}

	/* Map the bootloader memory. */
	int ret = map_range(0, (void *) bootloader_mem_size, 0, (void *) bootloader_mem_size, 2);
	if (ret) {
		printerr(error_messages[ret-1]);
		halt();
	}
	set_up_page_directory();
	enable_paging();

	/* Parse and run the kernel ELF. */
	ret = execute_elf(kernel, e820_map, e820_map_size);
	if (ret) {
		printerr(error_messages[ret-1]);
		halt();
	}
}
