#ifndef X86_IDT_H
#define X86_IDT_H

#include <stdint.h>
#include <irq/interrupt.h>

#define IDT_VECTOR_COUNT 256 /* our IDT will have 256 vectors */
#define IDT_DESCRIPTOR_SIZE 8 /* each IDT descriptor is 8 bytes long */

#define IDT_TRAP_GATE 		0xF
#define IDT_INTERRUPT_GATE 	0xE

struct idt_descriptor {
	uint16_t offset_1; /* bits 0-15 of offset */
	uint16_t segsel; /* segment selector */
	unsigned unused : 8, type : 5, dpl : 2, present : 1;
	uint16_t offset_2; /* bits 16-31 of offset */
} __attribute__((packed));

struct idtr {
	uint16_t limit; /* size of IDT minus 1 */
	uint32_t base; /* starting address of IDT */
} __attribute__((packed));

extern void idt_set_descriptor(struct idt_descriptor *idt, uint8_t vector, uint16_t segment, void (*offset)(void), uint8_t type, uint8_t dpl);
extern inline void load_idt(struct idtr idtr);
extern inline void populate_idtr(struct idtr *idtr, struct idt_descriptor *idt);

#endif
