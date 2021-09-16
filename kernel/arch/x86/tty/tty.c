#include <io.h>
#include <tty.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile char *video_memory = (char *) 0xB8000; /* VGA VRAM starts at 0xB8000 */

static int cursor_x = 0; /* keep track of where cursor is */
static int cursor_y = 0;

char *hex_chars = "0123456789ABCDEF";

void screen_clear(void)
{
	int x, y;
	for ( y = 0; y < VGA_HEIGHT; y++ ) {
		for ( x = 0; x < VGA_WIDTH; x++ ) {
			video_memory[(y * VGA_WIDTH + x) * 2 + 1] = VGA_COLOR_LIGHT_GRAY;
			video_memory[(y * VGA_WIDTH + x) * 2] = ' ';
		}
	}

	cursor_x = 0;
	cursor_y = 0;
}

void scroll_up(void)
{
	int x, y;
	for ( y = 1; y < VGA_HEIGHT; y++ ) {
		for ( x = 0; x < VGA_WIDTH; x++ ) {
			video_memory[((y - 1) * VGA_WIDTH + x) * 2] = video_memory[(y * VGA_WIDTH + x) * 2];
		}
	}
	for ( x = 0; x < VGA_WIDTH; x++ ) {
		video_memory[((VGA_HEIGHT - 1) * VGA_WIDTH + x) * 2] = ' ';
	}
	cursor_y = VGA_HEIGHT - 1;
}

void kprint(const char *string, uint8_t color)
{
	char next_char;
	uint8_t vga_misc_output;
	uint16_t crtc_port;
	next_char = *string;

	while ( next_char != 0 ) {
		if ( next_char == '\n') { cursor_x = 0; cursor_y++; }
		else { video_memory[(cursor_y * VGA_WIDTH + cursor_x) * 2] = next_char; video_memory[((cursor_y * VGA_WIDTH + cursor_x++) * 2)+ 1] = color != 0 ? color : VGA_COLOR_LIGHT_GRAY; }
		if ( cursor_x >= VGA_WIDTH ) { cursor_x = 0; cursor_y++; }
		if ( cursor_y >= VGA_HEIGHT ) { scroll_up(); }
		next_char = *++string;
	}

	vga_misc_output = inb(0x3CC);
	if (vga_misc_output & 0x1 == 0) {
		crtc_port = 0x3B4;
	} else {
		crtc_port = 0x3D4;
	}

	outb(crtc_port, 0xE);
	outb(crtc_port + 1, (cursor_y * VGA_WIDTH + cursor_x) >> 8);
	outb(crtc_port, 0xF);
	outb(crtc_port + 1, (cursor_y * VGA_WIDTH + cursor_x) & 0xFF);
}

void kprintc(const char character, uint8_t color)
{
	uint8_t vga_misc_output;
	uint16_t crtc_port;

	if ( character == '\n') { cursor_x = 0; cursor_y++; }
	else { video_memory[(cursor_y * VGA_WIDTH + cursor_x) * 2] = character; video_memory[((cursor_y * VGA_WIDTH + cursor_x++) * 2)+ 1] = color != 0 ? color : VGA_COLOR_LIGHT_GRAY; }
	if ( cursor_x >= VGA_WIDTH ) { cursor_x = 0; cursor_y++; }
	if ( cursor_y >= VGA_HEIGHT ) { scroll_up(); }

	vga_misc_output = inb(0x3CC);
	if (vga_misc_output & 0x1 == 0) {
		crtc_port = 0x3B4;
	} else {
		crtc_port = 0x3D4;
	}

	outb(crtc_port, 0xE);
	outb(crtc_port + 1, (cursor_y * VGA_WIDTH + cursor_x) >> 8);
	outb(crtc_port, 0xF);
	outb(crtc_port + 1, (cursor_y * VGA_WIDTH + cursor_x) & 0xFF);

}

void kprintb(uint8_t byte)
{
	uint8_t temp;
	temp = byte >> 4;
	kprint("0x", VGA_COLOR_LIGHT_GRAY);
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
	temp = byte & 0xF;
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
}

void kprintw(uint16_t word)
{
	uint8_t temp;
	temp = word >> 12;
	kprint("0x", VGA_COLOR_LIGHT_GRAY);
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
	temp = (word >> 8) & 0xF;
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
	temp = (word >> 4) & 0xF;
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
	temp = word & 0xF;
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
}

void kprintd(uint32_t dword)
{
	uint8_t temp;
	temp = dword >> 28;
	kprint("0x", VGA_COLOR_LIGHT_GRAY);
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
	temp = (dword >> 24) & 0xF;
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
	temp = (dword >> 20) & 0xF;
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
	temp = (dword >> 16) & 0xF;
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
	temp = (dword >> 12) & 0xF;
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
	temp = (dword >> 8) & 0xF;
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
	temp = (dword >> 4) & 0xF;
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
	temp = dword & 0xF;
	kprintc(hex_chars[temp], VGA_COLOR_LIGHT_GRAY);
}
