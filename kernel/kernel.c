#include <tty.h>

void _start(void)
{
	screen_clear();
	kprint("Hello there!\n\n\
			Hopefully your machine manages to print this text.\n\
			If it did, that's great news because I managed to write a partial VGA driver.\n\n\
			Right now, the short-term roadmap is as follows:\n\n\
			    * Enable interrupts using the PIC.\n\
				* Write a driver for the Intel 8042 PS/2 controller so the OS can receive keystrokes.\n\
				* A working i8042 driver will also aid in enabling A20 Gate, which should be done ASAP so the OS can address odd (as in odd/even number) megabytes.\n\n\
			Feel free to mess around with the code, although I doubt it will be very interesting at the moment.\n");
}
