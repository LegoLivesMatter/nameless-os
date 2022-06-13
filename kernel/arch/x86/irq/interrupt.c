#include <irq/idt.h>
#include <irq/interrupt.h>
#include <tty.h>
#include <stddef.h>
#include <irq/i8259a.h>

/* This table will hold pointers to our interrupt handlers. */
static int (*int_handler_table[256])(void);

void int_handler(int interrupt)
{
	if (int_handler_table[interrupt] == NULL) {
		kprint("WARNING: Unhandled interrupt ", 0);
		kprintb(interrupt);
		kprint(" occurred!\n", 0);
	} else {
		int ret = (*int_handler_table[interrupt])();
		if (ret) {
			kprint("WARNING: Error while handling interrupt ", 0);
			kprintb(interrupt);
			kprint("!\n", 0);
		}
	}
	if (interrupt >= 0x20) {
		pic_send_eoi(interrupt - 0x20);
	}
}

int register_interrupt(int irq, int (*handler)(void))
{
	int_handler_table[irq] = handler;
	if (irq >= 32 && irq < 48) {
		pic_unmask(irq-32);
	}
	return 0;
}
