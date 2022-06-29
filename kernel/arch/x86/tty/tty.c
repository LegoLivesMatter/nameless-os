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

void kprintnibble(uint8_t nibble)
{
	nibble &= 0xf;
	if (nibble < 0xa)
		kprintc('0' + nibble, 0);
	else kprintc('a' - 0xa + nibble, 0);
}

void kprintb(uint8_t byte, int print_delm)
{
	if (print_delm)
		kprint("0x", 0);

	kprintnibble(byte >> 4);
	kprintnibble(byte);
}

void kprintw(uint16_t word, int print_delm)
{
	if (print_delm)
		kprint("0x", 0);

	kprintb(word >> 8, 0);
	kprintb(word, 0);
}

void kprintd(uint32_t dword, int print_delm)
{
	if (print_delm)
		kprint("0x", 0);

	kprintw(dword >> 16, 0);
	kprintw(dword, 0);
}

void kprintq(uint64_t qword, int print_delm)
{
	if (print_delm)
		kprint("0x", 0);

	kprintd(qword >> 32, 0);
	kprintd(qword, 0);
}

int kprintdec(uint32_t num, uint8_t color)
{
	char buffer[11];
	int digits = 10;
	/* TODO: make an actual memset function to use instead of this */
	for (int i=0; i<11; i++) {
		buffer[i] = 0;
	}

	/* put the numbers in the buffer */
	for (int i=9; i>0 && num>0; i--) {
		uint8_t currdigit = num%10;
		buffer[i] = currdigit+'0';
		num /= 10;
	}

	/* shift the array as much as needed */
	while (*buffer == '\0') {
		digits--;
		for (int i=0; i<9; i++) {
			buffer[i] = buffer[i+1];
		}
	}

	/* zero out any leftovers */
	if (digits < 10) {
		for (int i=digits; i<10; i++) {
			buffer[i] = '\0';
		}
	}

	kprint(buffer, color);
	return digits;
}
