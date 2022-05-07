#include <tty.h>
#include <io.h>
#include <irq/idt.h>
#include <irq/i8259a.h>
#include <stdint.h>

extern void double_fault(struct abort_frame *frame);
extern void keyb_handler(struct interrupt_frame *frame);
struct idt_descriptor idt[256] __attribute__((aligned(0x10)));
struct idtr idtr __attribute__((aligned(0x10)));

void kmain(void)
{
	screen_clear();
	kprint("Welcome to Nameless OS!\nRunning revision: ", 0);
	kprint(GIT_COMMIT, 0);
	kprint("\n", 0);
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
