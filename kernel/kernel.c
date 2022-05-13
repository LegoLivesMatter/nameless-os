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

static struct page_directory_entry page_dir[1024] __attribute__((aligned(0x1000)));
static struct page_table_entry page_table[1024] __attribute__((aligned(0x1000)));

void kmain(void)
{
	screen_clear();
	kprint("Welcome to Nameless OS!\nRunning revision: ", 0);
	kprint(GIT_COMMIT, 0);
	kprint("\nEnabling paging...\n", 0);
	for (int i=0xb8; i < 0xc0; i++) {
		page_table[i].p = 1;
		page_table[i].rw = 0;
		page_table[i].us = 0;
		page_table[i].page_frame_addr = i;
	}
	for (int i=0x100; i < 0x102; i++) {
		page_table[i].p = 1;
		page_table[i].rw = 0;
		page_table[i].us = 0;
		page_table[i].page_frame_addr = i;
	}
	for (int i=0x102; i < 0x110; i++) {
		page_table[i].p = 1;
		page_table[i].rw = 1;
		page_table[i].us = 0;
		page_table[i].page_frame_addr = i;
	}
	page_dir[0].p = 1;
	page_dir[0].page_table_addr = (int) &page_table >> 12;
	enable_paging(page_dir);
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
	struct idtr curr_idtr;
	asm ("sidt %0": "=m" (curr_idtr));
	kprintw(curr_idtr.limit);
	kprintd(curr_idtr.base);
	//kprint("Gonna force a double fault\n", 0);
	//int test = 1/0;
	//int test2 = *(int *) (0xa0000000);
	while(1);
}
