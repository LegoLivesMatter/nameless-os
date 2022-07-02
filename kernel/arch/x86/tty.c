#include <io.h>
#include <tty.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static int cursor_x = 0; /* keep track of where cursor is */
static int cursor_y = 0;
static uint16_t crtc_port;

static inline uint8_t vga_get_color(const enum vga_color fg, const enum vga_color bg)
{
	return fg&0xf | bg<<4;
}

static inline uint16_t vga_get_char(const enum vga_color fg, const enum vga_color bg, char character)
{
	return character | vga_get_color(fg, bg)<<8;
}

static inline char *vga_get_memory_address(int x, int y)
{
	return (char *) 0xb8000 + (y*VGA_WIDTH + x)*2;
}

static void vga_scroll_up(void)
{
	for (int y=1; y<VGA_HEIGHT; y++) {
		for (int x = 0; x<VGA_WIDTH; x++) {
			*vga_get_memory_address(x, y-1) = *vga_get_memory_address(x, y);
		}
	}

	for (int x = 0; x<VGA_WIDTH; x++) {
		*vga_get_memory_address(x, VGA_HEIGHT-1) = vga_get_char(VGA_LIGHT_GRAY, VGA_BLACK, ' ');
	}
	cursor_y = VGA_HEIGHT - 1;
}

static void vga_set_cursor(int x, int y)
{
	uint16_t cursor_address = y*VGA_WIDTH + x;

	outb(crtc_port, 0xe);
	outb(crtc_port+1, cursor_address>>8);
	outb(crtc_port, 0xf);
	outb(crtc_port+1, cursor_address);
}

static void vga_screen_clear(void)
{
	for (int y=0; y<VGA_HEIGHT; y++) {
		for (int x=0; x<VGA_WIDTH; x++) {
			*vga_get_memory_address(x, y) = vga_get_char(VGA_LIGHT_GRAY, VGA_BLACK, ' ');
		}
	}

	cursor_x = 0;
	cursor_y = 0;
	vga_set_cursor(0, 0);
}

void vga_print_char(const char character, const enum vga_color fg, const enum vga_color bg)
{
	if (character == '\n') {
		cursor_x = 0;
		cursor_y++;
	} else {
		*vga_get_memory_address(cursor_x, cursor_y) = vga_get_char(fg, bg, character);
		cursor_x++;
	}

	if (cursor_x >= VGA_WIDTH) {
		cursor_x = 0;
		cursor_y++;
	}

	if (cursor_y >= VGA_HEIGHT)
		vga_scroll_up();

	vga_set_cursor(cursor_x, cursor_y);
}

void vga_initialize()
{
	uint8_t vga_misc = inb(0x3cc);
	if (vga_misc&1)
		crtc_port = 0x3d4;
	else
		crtc_port = 0x3b4;

	vga_screen_clear();
}
