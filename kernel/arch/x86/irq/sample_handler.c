#include <tty.h>
#include <irq/i8259a.h>
#include <io.h>
#include <stdint.h>
#include <mm/paging.h>
#include <panic.h>

typedef uint32_t uword_t;

struct interrupt_frame {
	uword_t ip;
	uword_t cs;
	uword_t flags;
};

struct fault_frame {
	struct pf_errcode error_code;
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
	int a = *(int *) (0xa0000000);
}

__attribute__((interrupt))
void pf_handler(struct fault_frame *frame)
{
	int address;
	struct pf_errcode errcode = frame->error_code;
	asm ("mov %%cr2, %0": "=a" (address));
	kprint("A page fault occurred!\n", VGA_COLOR_BRIGHT_RED);
	kprint("Faulting address: ", 0);
	kprintd(address);
	kprint("\n", 0);
	if (!errcode.p) {
		kprint("Address points to non-mapped page\n", 0);
	}
	if (errcode.wr) {
		kprint("Fault occurred while writing to memory\n", 0);
	} else {
		kprint("Fault occurred while reading from memory\n", 0);
	}
	if (errcode.id) {
		kprint("Fault occurred while fetching instruction\n", 0);
	}
	PANIC("page fault");
}

__attribute__((interrupt))
void double_fault(struct abort_frame *frame)
{
	PANIC("double fault");
}

