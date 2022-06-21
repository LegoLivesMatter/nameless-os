#ifndef BOOT_X86_ELF_H
#define BOOT_X86_ELF_H

#include <stdint.h>

/* Some constants we will test against. */
#define ELF_MAGIC 0x464c457f
#define ELF_32BIT 0x1
#define ELF_LITTLE_ENDIAN 0x1
#define ELF_SYSV_ABI 0x0
#define ELF_TYPE_EXECUTABLE 0x2
#define ELF_X86_ISA 0x3

#define ELF_PT_LOAD 0x1

#define ELF_FLAG_EXECUTABLE 0x1
#define ELF_FLAG_WRITABLE 0x2
#define ELF_FLAG_READABLE 0x4

/* Return error codes for the functions. */
#define ELF_HEADER_INVALID 5
#define MISMATCHED_SYSTEM 6
#define NOT_EXECUTABLE 7

struct elf_header {
	uint32_t magic;
	uint8_t bits;
	uint8_t endianness;
	uint8_t header_ver; /* always 1 */
	uint8_t os_abi;
	uint8_t padding[8];
	uint16_t type;
	uint16_t machine;
	uint32_t elf_ver; /* always 1 */
	void (*entry)(void *, int);
	uint32_t program_hdr; /* offset into file */
	uint32_t section_hdr;
	uint32_t flags;
	uint16_t hdr_size;
	uint16_t ph_entry_size;
	uint16_t ph_entry_count;
	uint16_t sh_entry_size;
	uint16_t sh_entry_count;
	uint16_t sh_name_index;
};

struct program_header {
	uint32_t seg_type;
	uint32_t file_offset;
	void *vaddr;
	uint32_t undefined;
	uint32_t filesz;
	uint32_t memsz;
	uint32_t flags;
	uint32_t align;
};

extern int execute_elf(struct elf_header *header, void *arg1, int arg2);

#endif
