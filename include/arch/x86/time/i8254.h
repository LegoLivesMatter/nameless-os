#ifndef X86_I8254_H
#define X86_I8254_H

#include <stdint.h>
#include <io.h>
#include <irq/i8259a.h>

/* for setting reload value and reading current count */
#define PORT_CHANNEL_0_DATA	0x40
#define PORT_CHANNEL_1_DATA	0x41
#define PORT_CHANNEL_2_DATA	0x42

/* for configuring the channels */
#define PORT_MODE_CMD_REG	0x43

/* operating modes */
#define INT_ON_TERM_CNT		0x0
#define HW_RETRIGGER_ONESHOT	0x1
#define RATE_GENERATOR		0x2
#define SQUARE_WAVE_GENERATOR	0x3
#define SW_TRIGGERED_STROBE	0x4
#define HW_TRIGGERED_STROBE	0x5

/* access modes */
#define	ACMODE_COUNTER_LATCH_CMD	0x0
#define ACMODE_LSB_ONLY			0x1
#define ACMODE_MSB_ONLY			0x2
#define ACMODE_LSB_MSB			0x3

union mode_command {
	struct {
		unsigned bcd: 1, /* if set, uses BCD for reload value */
			 opmode: 3, /* operating mode */
			 acmode: 2, /* access mode */
			 channel: 2; /* channel to configure */
	} fields;
	uint8_t command;
};

extern unsigned int ticks;
extern uint16_t reload;

extern void i8254_configure_channel(char channel, char opmode, uint16_t new_reload);
extern void i8254_irq_enable();

#endif
