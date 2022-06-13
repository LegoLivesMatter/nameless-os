#include <tty.h>
#include <io.h>
#include <input/ps2.h>
#include <bitflags.h>
#include <irq/interrupt.h>

static int was_released = 0, is_caps = 0;

int ps2_keyb_handler()
{
	uint8_t scancode = inb(PS2_DATA_PORT);
	if (was_released) {
		was_released = 0;
		return 0;
	}

	if (scancode == 0xf0) {
		was_released = 1;
		uint8_t scancode = inb(PS2_DATA_PORT);
		if (scancode == 0x12) {
			is_caps = 0;
		}
		return 0;
	} else {
		if (scancode == 0x12) {
			is_caps = 1;
			return 0;
		}
		if (!is_caps) {
			kprintc(scancodes[scancode], 0);
		} else {
			kprintc(scancodes[scancode] - ('a'-'A'), 0);
		}
	}
	return 0;
}

int ps2_initialize()
{
	uint8_t ccb, is_2channel, port_1_test, port_2_test;

	kprint("ps2: Begin initializing PS/2 controller\n", 0);
	outb(PS2_CMD_STS_PORT, PS2_CMD_PORT_2_DISABLE);
	ps2_input_wait();
	outb(PS2_CMD_STS_PORT, PS2_CMD_PORT_1_DISABLE);
	ps2_input_wait();
	inb(PS2_DATA_PORT);

	outb(PS2_CMD_STS_PORT, PS2_CMD_READ_CCB);
	ps2_output_wait();
	ccb = inb(PS2_DATA_PORT);

	CLEAR(ccb, 0);
	CLEAR(ccb, 1);
	CLEAR(ccb, 6);

	ps2_input_wait();
	outb(PS2_CMD_STS_PORT, PS2_CMD_WRITE_CCB);
	ps2_input_wait();
	outb(PS2_DATA_PORT, ccb);

	ps2_input_wait();
	outb(PS2_CMD_STS_PORT, PS2_CMD_CONTROLLER_TEST);
	ps2_output_wait();
	if (inb(PS2_DATA_PORT) != PS2_CONTROLLER_GOOD) {
		kprint("ps2: Controller self test failed, exiting!\n", 0);
		return -1;
	};

	ps2_input_wait();
	outb(PS2_CMD_STS_PORT, PS2_CMD_PORT_2_ENABLE);
	ps2_input_wait();
	outb(PS2_CMD_STS_PORT, PS2_CMD_READ_CCB);
	ps2_output_wait();
	ccb = inb(PS2_DATA_PORT);

	if (ccb & PORT_2_CLK != PORT_2_CLK) {
		is_2channel = 0;
		kprint("ps2: Controller is single-channel\n", 0);
	} else {
		is_2channel = 1;
		kprint("ps2: Controller is dual-channel\n", 0);
		ps2_input_wait();
		outb(PS2_CMD_STS_PORT, PS2_CMD_PORT_2_DISABLE);
	}

	ps2_input_wait();
	outb(PS2_CMD_STS_PORT, PS2_CMD_PORT_1_TEST);
	ps2_output_wait();

	if (inb(PS2_DATA_PORT) != 0) {
		port_1_test = 0;
		kprint("ps2: Port 1 test failed!\n", 0);
		if (!is_2channel) {
			kprint("ps2: No functional port, exiting!\n", 0);
			return -1;
		}
	} else port_1_test = 1;

	ps2_input_wait();
	if (is_2channel) {
		outb(PS2_CMD_STS_PORT, PS2_CMD_PORT_2_TEST);
		ps2_output_wait();

		if (inb(PS2_DATA_PORT) != 0) {
			port_2_test = 0;
			kprint("ps2: Port 2 test failed!", 0);
			if (!port_1_test) {
				kprint("ps2: No functional port, exiting!\n", 0);
				return -1;
			}
		} else port_2_test = 1;
	}

	if (port_1_test) { ps2_input_wait(); outb(PS2_CMD_STS_PORT, PS2_CMD_PORT_1_ENABLE); }
	if (port_2_test) { ps2_input_wait(); outb(PS2_CMD_STS_PORT, PS2_CMD_PORT_2_ENABLE); }

	int dev_1_test, dev_2_test;

	if (port_1_test) {
		uint8_t resp;

		do {
			ps2_input_wait();
			outb(PS2_DATA_PORT, PS2_DEV_RESET);
			ps2_output_wait();
			resp = inb(PS2_DATA_PORT);
		} while (resp == RESEND);

		if (resp == SELF_TEST_BAD || resp == SELF_TEST_BAD_2) {
			dev_1_test = 0;
			kprint("ps2: Port 1 device self test failed!\n", 0);
			if (!port_2_test) {
				kprint("ps2: No functioning devices, exiting!\n", 0);
				return -1;
			}
		} else { dev_1_test = 1; kprint("ps2: Port 1 device test successful\n", 0); }
	}

	inb(PS2_DATA_PORT);
	ps2_input_wait();

	if (port_2_test) {
		uint8_t resp;

		do {
			ps2_input_wait();
			outb(PS2_CMD_STS_PORT, PS2_CMD_PORT_2_WRITE);
			ps2_input_wait();
			outb(PS2_DATA_PORT, PS2_DEV_RESET);
			ps2_output_wait();
			resp = inb(PS2_DATA_PORT);
		} while (resp == RESEND);

		if (resp == SELF_TEST_BAD || resp == SELF_TEST_BAD_2) {
			dev_2_test = 0;
			kprint("ps2: Port 2 device self test failed!\n", 0);
			if (!dev_1_test) {
				kprint("ps2: No functioning devices, exiting!\n", 0);
				return -1;
			}
		} else { dev_2_test = 1; kprint("ps2: Port 2 device test successful\n", 0); }
	}

	inb(PS2_DATA_PORT);
	inb(PS2_DATA_PORT);
	ps2_input_wait();
	outb(PS2_CMD_STS_PORT, PS2_CMD_READ_CCB);
	ps2_output_wait();
	ccb = inb(PS2_DATA_PORT);

	SET(ccb, PORT_1_IRQ);

	ps2_input_wait();
	outb(PS2_CMD_STS_PORT, PS2_CMD_WRITE_CCB);
	ps2_input_wait();
	outb(PS2_DATA_PORT, ccb);

	if (dev_1_test) register_interrupt(33, &ps2_keyb_handler);

	if (dev_2_test && dev_1_test) return 1;
	else return 0;
}

void ps2_output_wait()
{
	uint8_t status;
	status = inb(PS2_CMD_STS_PORT);

	while (!IS_SET(status, OUT_STATUS)) status = inb(PS2_CMD_STS_PORT);
	return;
}

void ps2_input_wait()
{
	uint8_t status;
	status = inb(PS2_CMD_STS_PORT);

	while (IS_SET(status, IN_STATUS)) status = inb(PS2_CMD_STS_PORT);
	return;
}
