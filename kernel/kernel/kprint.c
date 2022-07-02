#include <stdint.h>
#include <kprint.h>

void tty_initialize()
{
	vga_initialize();
}

void kprint(const char *string, const enum vga_color fg, const enum vga_color bg)
{
	char character = *string;
	for (int i=1; character; i++) {
		vga_print_char(character, fg, bg);
		character = string[i];
	}
}

void kprintc(const char character, const enum vga_color fg, const enum vga_color bg)
{
	vga_print_char(character, fg, bg);
}

void kprintnibble(uint8_t nibble)
{
	nibble &= 0xf;
	if (nibble < 0xa)
		kprintc('0' + nibble, VGA_LIGHT_GRAY, VGA_BLACK);
	else kprintc('a' - 0xa + nibble, VGA_LIGHT_GRAY, VGA_BLACK);
}

void kprintb(const uint8_t byte, const int print_delm)
{
	if (print_delm)
		kprint("0x", VGA_LIGHT_GRAY, VGA_BLACK);

	kprintnibble(byte >> 4);
	kprintnibble(byte);
}

void kprintw(const uint16_t word, const int print_delm)
{
	if (print_delm)
		kprint("0x", VGA_LIGHT_GRAY, VGA_BLACK);

	kprintb(word >> 8, 0);
	kprintb(word, 0);
}

void kprintd(const uint32_t dword, const int print_delm)
{
	if (print_delm)
		kprint("0x", VGA_LIGHT_GRAY, VGA_BLACK);

	kprintw(dword >> 16, 0);
	kprintw(dword, 0);
}

void kprintq(const uint64_t qword, const int print_delm)
{
	if (print_delm)
		kprint("0x", VGA_LIGHT_GRAY, VGA_BLACK);

	kprintd(qword >> 32, 0);
	kprintd(qword, 0);
}

int kprintdec(uint32_t num, const enum vga_color fg, const enum vga_color bg)
{
	char buffer[11];
	int digits = 10;
	/* TODO: make an actual memset function to use instead of this */
	for (int i=0; i<11; i++) {
		buffer[i] = 0;
	}

	/* put the numbers in the buffer */
	for (int i=9; i>0 && num>0; i--) {
		const uint8_t currdigit = num%10;
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

	kprint(buffer, fg, bg);
	return digits;
}
