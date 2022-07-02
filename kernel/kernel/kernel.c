#include <kprint.h>
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
	kprint("       Base       |      Length      |   Type   |  Attrib  |\n", VGA_LIGHT_GRAY, VGA_BLACK);
	kprint("------------------------------------------------------------\n", VGA_LIGHT_GRAY, VGA_BLACK);
	for (int i=0; i<mmap_size; i++) {
		kprintq(mmap[i].base, 1);
		kprintc('|', VGA_LIGHT_GRAY, VGA_BLACK);
		kprintq(mmap[i].length, 1);
		kprintc('|', VGA_LIGHT_GRAY, VGA_BLACK);
		kprintd(mmap[i].type, 1);
		kprintc('|', VGA_LIGHT_GRAY, VGA_BLACK);
		kprintd(mmap[i].attrib, 1);
		kprintc('|', VGA_LIGHT_GRAY, VGA_BLACK);
		kprintc('\n', VGA_LIGHT_GRAY, VGA_BLACK);
	}
}

/* Small handler for double faults. Will be put elsewhere eventually. */
int double_fault_handler(struct interrupt_frame *frame)
{
	kprint("Double fault occurred!\n", VGA_BRIGHT_RED, VGA_BLACK);
	kprint("EAX: ", VGA_LIGHT_GRAY, VGA_BLACK);
	kprintd(frame->eax, 1);
	kprintc(' ', VGA_LIGHT_GRAY, VGA_BLACK);
	kprint("EBX: ", VGA_LIGHT_GRAY, VGA_BLACK);
	kprintd(frame->ebx, 1);
	kprintc(' ', VGA_LIGHT_GRAY, VGA_BLACK);
	kprint("ECX: ", VGA_LIGHT_GRAY, VGA_BLACK);
	kprintd(frame->ecx, 1);
	kprintc(' ', VGA_LIGHT_GRAY, VGA_BLACK);
	kprint("EDX: ", VGA_LIGHT_GRAY, VGA_BLACK);
	kprintd(frame->edx, 1);
	kprintc('\n', VGA_LIGHT_GRAY, VGA_BLACK);
	kprint("EBP: ", VGA_LIGHT_GRAY, VGA_BLACK);
	kprintd(frame->ebp, 1);
	kprintc(' ', VGA_LIGHT_GRAY, VGA_BLACK);
	kprint("ESP: ", VGA_LIGHT_GRAY, VGA_BLACK);
	kprintd(frame->esp, 1);
	kprintc('\n', VGA_LIGHT_GRAY, VGA_BLACK);
	kprint("ESI: ", VGA_LIGHT_GRAY, VGA_BLACK);
	kprintd(frame->esi, 1);
	kprintc(' ', VGA_LIGHT_GRAY, VGA_BLACK);
	kprint("EDI: ", VGA_LIGHT_GRAY, VGA_BLACK);
	kprintd(frame->edi, 1);
	kprintc('\n', VGA_LIGHT_GRAY, VGA_BLACK);
	kprint("EIP: ", VGA_LIGHT_GRAY, VGA_BLACK);
	kprintd(frame->eip, 1);
	kprintc(' ', VGA_LIGHT_GRAY, VGA_BLACK);
	kprint("EFLAGS: ", VGA_LIGHT_GRAY, VGA_BLACK);
	kprintd(frame->eflags, 1);
	kprintc('\n', VGA_LIGHT_GRAY, VGA_BLACK);
	PANIC("double fault");
}

void kmain(struct e820_map *mmap, int mmap_size)
{
	vga_initialize();
	kprint("Welcome to Nameless OS!\nRunning revision: ", VGA_LIGHT_GRAY, VGA_BLACK);
	kprint(GIT_COMMIT, VGA_LIGHT_GRAY, VGA_BLACK);
	kprint("\n", VGA_LIGHT_GRAY, VGA_BLACK);
	kprint("BIOS memory map:\n", VGA_LIGHT_GRAY, VGA_BLACK);
	print_e820(mmap, mmap_size);
	kprint("Preparing IDT...\n", VGA_LIGHT_GRAY, VGA_BLACK);
	for (int i=0; i<48; i++) {
		idt_set_descriptor(idt, i, 0x8, _int_handler_table[i], IDT_INTERRUPT_GATE, 0);
	}
	kprint("IDT prepared, loading...\n", VGA_LIGHT_GRAY, VGA_BLACK);
	populate_idtr(&idtr, idt);
	load_idt(idtr);
	kprint("Setting up interrupts...\n", VGA_LIGHT_GRAY, VGA_BLACK);
	register_interrupt(8, &double_fault_handler);
	pic_init(0x20, 0x28);
	pic_mask_all();
	asm volatile ("sti");
	kprint("All done\n", VGA_LIGHT_GRAY, VGA_BLACK);
	while(1);
}
