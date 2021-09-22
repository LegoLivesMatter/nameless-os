#ifndef X86_I8259A_H
#define X86_I8259A_H

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define PIC_ICW1_INIT 0x10
#define PIC_ICW1_ICW4 0x1

#define PIC_ICW4_8086 0x1

#define PIC_EOI 0x20

extern void pic_init(int offset1, int offset2);
extern void pic_mask(uint8_t irq);
extern void pic_unmask(uint8_t irq);
extern void pic_mask_all(void);
extern void pic_unmask_all(void);
extern void pic_send_eoi(uint8_t irq);

#endif
