#include <tty.h>
#include <irq/i8259a.h>
#include <io.h>
#include <stdint.h>
#include <input/ps2.h>

int was_released = 0;
int is_caps = 0;

typedef uint32_t uword_t;

struct interrupt_frame {
	uword_t ip;
	uword_t cs;
	uword_t flags;
};

struct abort_frame;

__attribute__((interrupt))
void keyb_handler(struct interrupt_frame *frame)
{
	pic_send_eoi(1);
	uint8_t scancode = inb(PS2_DATA_PORT);
	if (was_released) {
		was_released = 0;
		return;
	}

	if (scancode == 0xf0) {
		was_released = 1;
		uint8_t scancode = inb(PS2_DATA_PORT);
		if (scancode == 0x12) {
			is_caps = 0;
		}
		return;
	} else {
		if (scancode == 0x12) {
			is_caps = 1;
			return;
		}
		if (!is_caps) {
			kprintc(scancodes[scancode], 0);
		} else {
			kprintc(scancodes[scancode] - ('a'-'A'), 0);
		}
	}
}


__attribute__((interrupt))
void double_fault(struct abort_frame *frame)
{
	*(volatile uint32_t *) (0xb8000) = 0xcf28cf3a;
halt:
	asm volatile ("cli; hlt");
	goto halt;
}

