#include <tty.h>
#include <irq/i8259a.h>
#include <io.h>
#include <stdint.h>
#include <mm/paging.h>

typedef uint32_t uword_t;

struct interrupt_frame {
	uword_t ip;
	uword_t cs;
	uword_t flags;
};

struct fault_frame {
	uint32_t error_code;
	uintptr_t eip;
	uint16_t cs;
	uint32_t eflags;
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
void pf_handler(struct fault_frame *frame)
{
	struct pf_errcode *errcode = &(frame->error_code);
	kprint("A page fault occurred!\n", VGA_COLOR_DARK_RED);
	if (errcode->p) {
		kprint("Attempted to access non-present page\n", 0);
	}
	if (errcode->wr) {
		kprint("Kernel attempted to write to page\n", 0);
	} else kprint("Kernel attempted to read from page\n", 0);
	if (errcode->id) {
		kprint("Fault occurred while fetching instruction\n", 0);
	}
	int cr2;
	asm ("mov %%cr2, %0": "=a" (cr2));
	kprint("CR2: ", 0);
	kprintd(cr2);
halt:
	asm ("cli; hlt");
	goto halt;
}

__attribute__((interrupt))
void double_fault(struct abort_frame *frame)
{
	*(volatile uint32_t *) (0xb8000) = 0xcf28cf3a;
halt:
	asm volatile ("cli; hlt");
	goto halt;
}

