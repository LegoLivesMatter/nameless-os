#include <tty.h>
#include <irq/i8259a.h>
#include <io.h>
#include <stdint.h>

typedef uint32_t uword_t;

struct interrupt_frame {
	uword_t ip;
	uword_t cs;
	uword_t flags;
};

struct abort_frame;

__attribute__((interrupt))
void keyb_handler(struct interrupt_frame *frame)
{
	pic_send_eoi(1);
	kprint("Got a keyboard interrupt!\n", 0);
	inb(0x60);
}


__attribute__((interrupt))
void double_fault(struct abort_frame *frame)
{
	*(volatile char *) (0xb8000) = ":";
	*(volatile char *) (0xb8002) = "(";
	asm volatile ("cli; hlt");
	while(1);
}

