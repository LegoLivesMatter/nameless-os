#ifndef PANIC_H
#define PANIC_H

extern void halt();

#define PANIC(msg) \
	kprint("PANIC (", VGA_BRIGHT_RED, VGA_BLACK);\
	kprint(msg, VGA_BRIGHT_RED, VGA_BLACK);\
	kprint(") in ", VGA_BRIGHT_RED, VGA_BLACK);\
	kprint(__FILE__, VGA_BRIGHT_RED, VGA_BLACK);\
	kprintc(':', VGA_BRIGHT_RED, VGA_BLACK);\
	kprintdec(__LINE__, VGA_BRIGHT_RED, VGA_BLACK);\
	kprintc(':', VGA_BRIGHT_RED, VGA_BLACK);\
	kprint(__func__, VGA_BRIGHT_RED, VGA_BLACK);\
	kprint("()", VGA_BRIGHT_RED, VGA_BLACK);\
	halt();

#endif
