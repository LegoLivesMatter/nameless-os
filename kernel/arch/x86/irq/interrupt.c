#include <irq/idt.h>
#include <irq/interrupt.h>
#include <tty.h>
#include <stddef.h>
#include <irq/i8259a.h>

/* This table will hold pointers to our interrupt handlers. */
static int (*int_handler_table[256])(struct interrupt_frame *);

void int_handler(struct interrupt_frame *frame)
{
	int interrupt = frame->interrupt;
	if (int_handler_table[interrupt] == NULL) {
		kprint("WARNING: Unhandled interrupt ", 0);
		kprintb(interrupt, 1);
		kprint(" occurred!\n", 0);
	} else {
		int ret = (*int_handler_table[interrupt])(frame);
		if (ret) {
			kprint("WARNING: Error while handling interrupt ", 0);
			kprintb(interrupt, 1);
			kprint("!\n", 0);
		}
	}
	if (interrupt >= 0x20) {
		pic_send_eoi(interrupt - 0x20);
	}
}

int register_interrupt(int irq, int (*handler)(struct interrupt_frame *))
{
	int_handler_table[irq] = handler;
	if (irq >= 32 && irq < 48) {
		pic_unmask(irq-32);
	}
	return 0;
}
