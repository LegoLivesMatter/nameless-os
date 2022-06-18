ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS {
	. = 0x1800;
	.text : {
		stage3/loader.o(.text)
		*(.text)
	}
	.rodata : { *(.rodata) }
	.data : { *(.data) }
	.bss : {
		*(.bss)
		. += 1K;
		PROVIDE(__STACK_BOTTOM__ = .);
	}
}
