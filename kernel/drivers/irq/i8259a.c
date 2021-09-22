#include <io.h>
#include <tty.h>
#include <irq/i8259a.h>

void pic_init(int offset1, int offset2)
{
	uint8_t a1, a2;

	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);
	io_wait();
	outb(PIC2_DATA, offset2);
	io_wait();
	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();

	outb(PIC1_DATA, PIC_ICW4_8086);
	io_wait();
	outb(PIC2_DATA, PIC_ICW4_8086);
	io_wait();

	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

void pic_mask(uint8_t irq)
{
	uint16_t port;
	uint8_t value;

	if (irq < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
	}

	value = inb(port) | (1 << irq);
	outb(port, value);
}

void pic_unmask(uint8_t irq)
{
	uint16_t port;
	uint8_t value;

	if (irq < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
	}

	value = inb(port) & ~(1 << irq);
	outb(port, value);
}

void pic_mask_all(void)
{
	outb(PIC1_DATA, 0xFF);
}

void pic_unmask_all(void)
{
	outb(PIC1_DATA, 0);
}

void pic_send_eoi(uint8_t irq)
{
	if (irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);

	outb(PIC1_COMMAND, PIC_EOI);
}
