#include <irq/idt.h>
#include <stdint.h>

/* Note to self: passing a pointer to an asm constraint as "m" will give a pointer to pointer */
inline void load_idt(struct idtr idtr)
{
	asm volatile ("lidt %0": : "m" (idtr));
}

void idt_set_descriptor(struct idt_descriptor *idt, uint8_t vector, uint16_t segment, uint32_t offset, uint8_t type, uint8_t dpl)
{
	struct idt_descriptor *descriptor = &idt[vector];

	descriptor->offset_1 = offset & 0xFFFF;
	descriptor->segsel = segment;
	descriptor->unused = 0;
	descriptor->type = type;
	descriptor->dpl = dpl;
	descriptor->present = 1;
	descriptor->offset_2 = offset >> 16;
}

inline void populate_idtr(struct idtr *idtr, struct idt_descriptor *idt)
{
	idtr->limit = ((IDT_VECTOR_COUNT * IDT_DESCRIPTOR_SIZE) - 1); /* limit must be size - 1 */
	idtr->base = (uint32_t) idt;
}
