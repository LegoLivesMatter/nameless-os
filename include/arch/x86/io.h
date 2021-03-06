#ifndef X86_IO_H
#define X86_IO_H

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t value)
{
	asm volatile ("outb %0, %1": : "a" (value), "Nd" (port));
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	asm volatile ("inb %1, %0"
					: "=a" (ret)
					: "d" (port));
	return ret;
}

static inline void io_wait(void)
{
	inb(0x80);
}

#endif
