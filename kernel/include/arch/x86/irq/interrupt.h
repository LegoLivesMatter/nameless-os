#ifndef X86_INTERRUPT_H
#define X86_INTERRUPT_H

struct interrupt_frame {
	int interrupt;
	int edi;
	int esi;
	int ebp;
	int esp; /* before pushad */
	int ebx;
	int edx;
	int ecx;
	int eax;
	int err_code;
	int eip;
	int cs;
	int eflags;
};

extern int register_interrupt(int irq, int (*handler)(struct interrupt_frame *));

#endif
