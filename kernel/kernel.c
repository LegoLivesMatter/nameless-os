#include <stdint.h>
#include <tty.h>
#include <io.h>
#include <irq/idt.h>
#include <irq/i8259a.h>
#include <input/ps2.h>

extern void double_fault(struct abort_frame *frame);
extern void keyb_handler(struct interrupt_frame *frame);
struct idt_descriptor idt[256] __attribute__((aligned(0x10)));
struct idtr idtr __attribute__((aligned(0x10)));

void kmain(void)
{
	int ps2_success;
	screen_clear();
	kprint("Welcome to Nameless OS!\nRunning revision: ", 0);
	kprint(GIT_COMMIT, 0);
	kprint("\n", 0);
	ps2_success = ps2_initialize();
	switch (ps2_success) {
		case -1:
			kprint("No PS/2 devices found or working, we will have no input\n", 0);
			break;
		case 0:
			kprint("Found one working PS/2 device\n", 0);
			break;
		case 1:
			kprint("Found two working PS/2 devices\n", 0);
	}
	kprint("Preparing IDT...\n", 0);
	idt_set_descriptor(idt, 0x8, 0x8, (uint32_t) double_fault, IDT_TRAP_GATE, 0x0);
	idt_set_descriptor(idt, 0x21, 0x8, (uint32_t) keyb_handler, IDT_INTERRUPT_GATE, 0x0);
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
