#include <tty.h>
#include <io.h>
#include <irq/idt.h>
#include <irq/i8259a.h>
#include <stdint.h>
#include <mm/paging.h>

extern void double_fault(struct abort_frame *frame);
extern void keyb_handler(struct interrupt_frame *frame);
extern void pf_handler(struct fault_frame *frame);
static struct idt_descriptor idt[256] __attribute__((aligned(0x10)));
static struct idtr idtr __attribute__((aligned(0x10)));

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
		kprintq(mmap[i].base);
		kprintc('|', 0);
		kprintq(mmap[i].length);
		kprintc('|', 0);
		kprintd(mmap[i].type);
		kprintc('|', 0);
		kprintd(mmap[i].attrib);
		kprintc('|', 0);
		kprintc('\n', 0);
	}
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
	idt_set_descriptor(idt, 8, 0x8, (uint32_t) double_fault, IDT_INTERRUPT_GATE, 0x0);
	idt_set_descriptor(idt, 14, 0x8, (uint32_t) pf_handler, IDT_INTERRUPT_GATE, 0x0);
	idt_set_descriptor(idt, 33, 0x8, (uint32_t) keyb_handler, IDT_INTERRUPT_GATE, 0x0);
	kprint("IDT prepared, loading...\n", 0);
	populate_idtr(&idtr, idt);
	load_idt(idtr);
	kprint("IDT loaded, enabling interrupts...\n", 0);
	pic_init(0x20, 0x28);
	pic_mask_all();
	pic_unmask(1);
	asm volatile ("sti");
	kprint("All done\n", 0);
	while(1);
}
