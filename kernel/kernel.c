#include <stdint.h>
#include <tty.h>
#include <io.h>
#include <irq/idt.h>
#include <irq/interrupt.h>
#include <irq/i8259a.h>
#include <input/ps2.h>
#include <stddef.h>

struct idt_descriptor idt[256] __attribute__((aligned(0x10)));
struct idtr idtr __attribute__((aligned(0x10)));
extern void (*_int_handler_table[48])(void);

/* Small handler for double faults. Will be put elsewhere eventually. */
int double_fault_handler(struct interrupt_frame *frame)
{
	kprint("PANIC: Double fault occurred!\n", VGA_COLOR_BRIGHT_RED);
	kprint("EAX: ", VGA_COLOR_BRIGHT_RED);
	kprintd(frame->eax);
	kprint("\n", VGA_COLOR_BRIGHT_RED);
	kprint("EBX: ", VGA_COLOR_BRIGHT_RED);
	kprintd(frame->ebx);
	kprint("\n", VGA_COLOR_BRIGHT_RED);
	kprint("ECX: ", VGA_COLOR_BRIGHT_RED);
	kprintd(frame->ecx);
	kprint("\n", VGA_COLOR_BRIGHT_RED);
	kprint("EDX: ", VGA_COLOR_BRIGHT_RED);
	kprintd(frame->edx);
	kprint("\n", VGA_COLOR_BRIGHT_RED);
	kprint("EBP: ", VGA_COLOR_BRIGHT_RED);
	kprintd(frame->ebp);
	kprint("\n", VGA_COLOR_BRIGHT_RED);
	kprint("ESP: ", VGA_COLOR_BRIGHT_RED);
	kprintd(frame->esp);
	kprint("\n", VGA_COLOR_BRIGHT_RED);
	kprint("ESI: ", VGA_COLOR_BRIGHT_RED);
	kprintd(frame->esi);
	kprint("\n", VGA_COLOR_BRIGHT_RED);
	kprint("EDI: ", VGA_COLOR_BRIGHT_RED);
	kprintd(frame->edi);
	kprint("\n", VGA_COLOR_BRIGHT_RED);
	kprint("EIP: ", VGA_COLOR_BRIGHT_RED);
	kprintd(frame->eip);
	kprint("\n", VGA_COLOR_BRIGHT_RED);
	kprint("EFLAGS: ", VGA_COLOR_BRIGHT_RED);
	kprintd(frame->eflags);
	kprint("\n", VGA_COLOR_BRIGHT_RED);

	/* IF has already been cleared for us */
halt:
	asm("hlt");
	goto halt;
}

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
	register_interrupt(8, &double_fault_handler);
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

	while(1) {
		char *string = NULL;
		string = kgets();
		kprint(string, VGA_COLOR_LIME);
		kprint("\n", 0);
	}
}
