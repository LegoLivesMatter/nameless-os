/* Code for parsing ELF binaries (the kernel) */

#include "elf.h"
#include "paging.h"
#include <stdint.h>

int check_elf_header(struct elf_header *header)
{
	if (header->magic != ELF_MAGIC)
		return ELF_HEADER_INVALID;
	if (header->bits != ELF_32BIT)
		return MISMATCHED_SYSTEM;
	if (header->endianness != ELF_LITTLE_ENDIAN)
		return MISMATCHED_SYSTEM;
	if (header->header_ver != 1)
		return ELF_HEADER_INVALID;
	if (header->os_abi != ELF_SYSV_ABI)
		return MISMATCHED_SYSTEM;
	if (header->type != ELF_TYPE_EXECUTABLE)
		return NOT_EXECUTABLE;
	if (header->machine != ELF_X86_ISA)
		return MISMATCHED_SYSTEM;
	if (header->elf_ver != 1)
		return ELF_HEADER_INVALID;

	return 0;
}

int setup_elf_executable(struct elf_header *header)
{
	/* Check the header. */
	int header_valid = check_elf_header(header);
	if (header_valid)
		return header_valid;

	/* Get the program header. The casting is necessary because without it
	 * the code will add some multiple of the program header offset rather
	 * than the actual offset. */
	struct program_header *prog_hdr = (struct program_header *) ((uint32_t) header+header->program_hdr);
	for (int i=0; i<header->ph_entry_count; i++) {
		/* We're only interested in loadable segments. */
		if (prog_hdr[i].seg_type != ELF_PT_LOAD)
			continue;

		/* Calculate the memory range. */
		void *phys_start, *phys_end, *virt_end;
		int flags;
		phys_start = (void *) header + prog_hdr[i].file_offset;
		if (prog_hdr[i].memsz & 0xfff) { /* Align if needed. */
			phys_end = phys_start+((prog_hdr[i].memsz+0x1000) & ~0xfff);
			virt_end = prog_hdr[i].vaddr+((prog_hdr[i].memsz+0x1000) & ~0xfff);
		}
		else {
			phys_end = phys_start+prog_hdr[i].memsz;
			virt_end = prog_hdr[i].vaddr+prog_hdr[i].memsz;
		}

		flags = prog_hdr[i].flags & ELF_FLAG_WRITABLE;

		/* Map the range. */
		int ret = map_range(phys_start, phys_end, prog_hdr[i].vaddr, virt_end, flags);
		if (ret)
			return ret;
	}

	return 0;
}

int execute_elf(struct elf_header *header, void *arg1, int arg2)
{
	/* Check the header and map all needed memory ranges. */
	int ret = setup_elf_executable(header);
	if (ret)
		return ret;

	/* Execute the kernel. */
	header->entry(arg1, arg2);
}
