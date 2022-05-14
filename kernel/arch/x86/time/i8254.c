#include <time/i8254.h>

uint16_t reload;

void i8254_configure_channel(char channel, char opmode, uint16_t new_reload)
{
	union mode_command command;

	command.fields.bcd = 0;
	command.fields.channel = channel;
	command.fields.acmode = ACMODE_LSB_MSB;
	command.fields.opmode = opmode;

	outb(PORT_MODE_CMD_REG, command.command);

	switch (channel) {
		case 0:
			outb(PORT_CHANNEL_0_DATA, new_reload & 0xff);
			outb(PORT_CHANNEL_0_DATA, new_reload >> 8);
			break;
		case 1:
			outb(PORT_CHANNEL_1_DATA, new_reload & 0xff);
			outb(PORT_CHANNEL_1_DATA, new_reload >> 8);
			break;
		case 2:
			outb(PORT_CHANNEL_2_DATA, new_reload & 0xff);
			outb(PORT_CHANNEL_2_DATA, new_reload >> 8);
			break;
	}

	reload = new_reload;
}

void i8254_irq_enable()
{
	pic_unmask(0);
}
