ENTRY(_start)

SECTIONS
{
	. = 0xc0000000;
	__KERNEL_BASE__ = .;

	.text : ALIGN(4K) {
		__TEXT_BASE__ = .;
		entry.o (.text)
		*(.text)
		__TEXT_END__ = .;
	}
	.rodata : ALIGN(4K) {
		__RODATA_BASE__ = .;
		*(.rodata)
		__RODATA_END__ = .;
	}

	/* .rodata is put after .text so that the read-only entry in the program
	 * header covers .rodata as well. When .rodata is after .data, that entry
	 * only covers .text, leaving .rodata in the writable program header
	 * entry. The problem with this approach is that the program header
	 * entry where .text and .rodata are is marked as executable, which
	 * would mean that .rodata should be executable too, but that doesn't
	 * matter yet because we (currently) assume that the NX bit is not
	 * supported anyway. */

	.data : ALIGN(4K) {
		__DATA_BASE__ = .;
		*(.data)
		__DATA_END__ = .;
	}
	.bss : ALIGN(4K) {
		__BSS_BASE__ = .;
		*(.bss)
		/* Reserving 16KiB for the stack. A __STACK_TOP__ is not really
		 * needed (yet). */
		. += 16K;
		__STACK_BOTTOM__ = .;
		__BSS_END__ = .;
	}
}
