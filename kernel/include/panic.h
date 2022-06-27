#ifndef PANIC_H
#define PANIC_H

extern void halt();

#define PANIC(msg) \
	kprint("PANIC (", VGA_COLOR_BRIGHT_RED);\
	kprint(msg, VGA_COLOR_BRIGHT_RED);\
	kprint(") in ", VGA_COLOR_BRIGHT_RED);\
	kprint(__FILE__, VGA_COLOR_BRIGHT_RED);\
	kprintc(':', VGA_COLOR_BRIGHT_RED);\
	kprintdec(__LINE__, VGA_COLOR_BRIGHT_RED);\
	kprintc(':', VGA_COLOR_BRIGHT_RED);\
	kprint(__func__, VGA_COLOR_BRIGHT_RED);\
	kprint("()", VGA_COLOR_BRIGHT_RED);\
	halt();

#endif
