#include <tty.h>
#include <io.h>
#include <irq/idt.h>
#include <irq/interrupt.h>
#include <irq/i8259a.h>
#include <stdint.h>
#include <mm/paging.h>
#include <panic.h>

struct idt_descriptor idt[256] __attribute__((aligned(0x10)));
struct idtr idtr __attribute__((aligned(0x10)));
extern void (*_int_handler_table[48])(void);

struct e820_map {
	uint64_t base;
	uint64_t length;
	uint32_t type;
	uint32_t attrib;
};

void print_e820(struct e820_map *mmap, int mmap_size)
{
	kprint("       Base       |      Length      |   Type   |  Attrib  |\n", 0);
	kprint("------------------------------------------------------------\n", 0);
	for (int i=0; i<mmap_size; i++) {
		kprintq(mmap[i].base, 1);
		kprintc('|', 0);
		kprintq(mmap[i].length, 1);
		kprintc('|', 0);
		kprintd(mmap[i].type, 1);
		kprintc('|', 0);
		kprintd(mmap[i].attrib, 1);
		kprintc('|', 0);
		kprintc('\n', 0);
	}
}

/* Small handler for double faults. Will be put elsewhere eventually. */
int double_fault_handler(struct interrupt_frame *frame)
{
	kprint("Double fault occurred!\n", VGA_COLOR_BRIGHT_RED);
	kprint("EAX: ", 0);
	kprintd(frame->eax, 1);
	kprintc(' ', 0);
	kprint("EBX: ", 0);
	kprintd(frame->ebx, 1);
	kprintc(' ', 0);
	kprint("ECX: ", 0);
	kprintd(frame->ecx, 1);
	kprintc(' ', 0);
	kprint("EDX: ", 0);
	kprintd(frame->edx, 1);
	kprintc('\n', 0);
	kprint("EBP: ", 0);
	kprintd(frame->ebp, 1);
	kprintc(' ', 0);
	kprint("ESP: ", 0);
	kprintd(frame->esp, 1);
	kprintc('\n', 0);
	kprint("ESI: ", 0);
	kprintd(frame->esi, 1);
	kprintc(' ', 0);
	kprint("EDI: ", 0);
	kprintd(frame->edi, 1);
	kprintc('\n', 0);
	kprint("EIP: ", 0);
	kprintd(frame->eip, 1);
	kprintc(' ', 0);
	kprint("EFLAGS: ", 0);
	kprintd(frame->eflags, 1);
	kprintc('\n', 0);
	PANIC("double fault");
}

void kmain(struct e820_map *mmap, int mmap_size)
{
	screen_clear();
	kprint("Welcome to Nameless OS!\nRunning revision: ", 0);
	kprint(GIT_COMMIT, 0);
	kprint("\n", 0);
	kprint("BIOS memory map:\n", 0);
	print_e820(mmap, mmap_size);
	kprint("Preparing IDT...\n", 0);
	for (int i=0; i<48; i++) {
		idt_set_descriptor(idt, i, 0x8, _int_handler_table[i], IDT_INTERRUPT_GATE, 0);
	}
	kprint("IDT prepared, loading...\n", 0);
	populate_idtr(&idtr, idt);
	load_idt(idtr);
	kprint("Setting up interrupts...\n", 0);
	register_interrupt(8, &double_fault_handler);
	pic_init(0x20, 0x28);
	pic_mask_all();
	asm volatile ("sti");
	kprint("All done\n", 0);
	while(1);
}
