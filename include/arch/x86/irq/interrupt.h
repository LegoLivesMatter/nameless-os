#ifndef X86_INTERRUPT_H
#define X86_INTERRUPT_H

extern int register_interrupt(int irq, int (*handler)(void));

#endif
