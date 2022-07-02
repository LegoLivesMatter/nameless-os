#ifndef X86_TTY_H
#define X86_TTY_H

enum vga_color {
	VGA_BLACK,
	VGA_BLUE,
	VGA_GREEN,
	VGA_TEAL,
	VGA_DARK_RED,
	VGA_MAGENTA,
	VGA_BROWN,
	VGA_LIGHT_GRAY,
	VGA_DARK_GRAY,
	VGA_PURPLE,
	VGA_LIME,
	VGA_CYAN,
	VGA_BRIGHT_RED,
	VGA_PINK,
	VGA_YELLOW,
	VGA_WHITE
};

extern void vga_initialize(void);
extern void vga_print_char(const char character, enum vga_color fg, enum vga_color bg);
#endif
