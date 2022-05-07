ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS
{
	. = 0x100000;
	__KERNEL_BASE__ = .;

	.text : ALIGN(4K) { 
		kernel/entry.o (.text)
		*(.text) 
	}
	.rodata : ALIGN(4K) { *(.rodata) }

	/* .rodata is put after .text so that the read-only entry in the program
	 * header covers .rodata as well. When .rodata is after .data, that entry
	 * only covers .text, leaving .rodata in the writable program header
	 * entry. The problem with this approach is that the program header
	 * entry where .text and .rodata are is marked as executable, which
	 * would mean that .rodata should be executable too, but that doesn't
	 * matter yet because we (currently) assume that the NX bit is not
	 * supported anyway. */

	.data : ALIGN(4K) { *(.data) }
	.bss : ALIGN(4K) { 
		*(.bss)
		/* Reserving 16KiB for the stack. A __STACK_TOP__ is not really
		 * needed (yet). */
		. += 16K; 
		__STACK_BOTTOM__ = .;
	}
}
