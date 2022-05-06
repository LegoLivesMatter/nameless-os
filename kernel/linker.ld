ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS
{
	. = 0x100000;

	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}
