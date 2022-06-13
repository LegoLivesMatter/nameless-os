#include <stdint.h>
#include <tty.h>
#include <io.h>
#include <irq/idt.h>
#include <irq/interrupt.h>
#include <irq/i8259a.h>
#include <input/ps2.h>

struct idt_descriptor idt[256] __attribute__((aligned(0x10)));
struct idtr idtr __attribute__((aligned(0x10)));
extern void (*_int_handler_table[48])(void);

void kmain(void)
{
	int ps2_success;
	screen_clear();
	kprint("Welcome to Nameless OS!\nRunning revision: ", 0);
	kprint(GIT_COMMIT, 0);
	kprint("\n", 0);
	kprint("Preparing IDT...\n", 0);
	for (int i=0; i<48; i++) {
		idt_set_descriptor(idt, i, 0x8, _int_handler_table[i], IDT_INTERRUPT_GATE, 0);
	}
	kprint("IDT prepared, loading...\n", 0);
	populate_idtr(&idtr, idt);
	load_idt(idtr);
	kprint("Setting up interrupts...\n", 0);
	pic_init(0x20, 0x28);
	pic_mask_all();
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
	asm volatile ("sti");
	kprint("All done\n", 0);
	while(1);
}
