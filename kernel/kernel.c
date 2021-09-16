#include <tty.h>

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
	kprint(string);
}
