#include <tty.h>
#include <io.h>
#include <stdint.h>

const char *string = "Hello there!\n\n\
Hopefully your machine manages to print this text.\n\
If it did, that's great news because I managed to write a partial VGA driver.\n\n\
Right now, the short-term roadmap is as follows:\n\n\
* Complete the text-mode part of the VGA driver.\n\
* Enable interrupts using the PIC.\n\
* Write a driver for the Intel 8042 PS/2 controller so the OS can receive keystrokes.\n\
* Once all that is done, it would be desirable to start the switch to userspace so the OS can actually be remotely usable.\n\n\
Feel free to mess around with the code, although I doubt it will be very interesting at the moment.\n";

void _start(void)
{
	screen_clear();
	/*kprint(string, 0x07);*/
	kprint("Color 0x01\n", VGA_COLOR_BLUE);
	kprint("Color 0x02\n", VGA_COLOR_GREEN);
	kprint("Color 0x03\n", VGA_COLOR_TEAL);
	kprint("Color 0x04\n", VGA_COLOR_DARK_RED);
	kprint("Color 0x05\n", VGA_COLOR_MAGENTA);
	kprint("Color 0x06\n", VGA_COLOR_BROWN);
	kprint("Color 0x07\n", VGA_COLOR_LIGHT_GRAY);
	kprint("Color 0x08\n", VGA_COLOR_DARK_GRAY);
	kprint("Color 0x09\n", VGA_COLOR_PURPLE);
	kprint("Color 0x0A\n", VGA_COLOR_LIME);
	kprint("Color 0x0B\n", VGA_COLOR_CYAN);
	kprint("Color 0x0C\n", VGA_COLOR_BRIGHT_RED);
	kprint("Color 0x0D\n", VGA_COLOR_PINK);
	kprint("Color 0x0E\n", VGA_COLOR_YELLOW);
	kprint("Color 0x0F\n", VGA_COLOR_WHITE);
	kprintb(0xAE);
	kprint("\n", 0);
	kprintw(0xDEAD);
	kprint("\n", 0);
	kprintd(0x89ABCDEF);
}
