#ifndef X86_INPUT_PS2_H
#define X86_INPUT_PS2_H

/* port constants */
#define PS2_DATA_PORT 0x60
#define PS2_CMD_STS_PORT 0x64

/* command constants */
#define PS2_CMD_PORT_2_DISABLE 0xA7
#define PS2_CMD_PORT_1_DISABLE 0xAD
#define PS2_CMD_PORT_2_ENABLE 0xA8
#define PS2_CMD_PORT_1_ENABLE 0xAE
#define PS2_CMD_READ_CCB 0x20
#define PS2_CMD_WRITE_CCB 0x60
#define PS2_CMD_CONTROLLER_TEST 0xAA
#define PS2_CMD_PORT_1_TEST 0xAB
#define PS2_CMD_PORT_2_TEST 0xA9
#define PS2_CMD_PORT_2_WRITE 0xD4

#define PS2_DEV_RESET 0xFF

#define PS2_CONTROLLER_GOOD 0x55
#define PS2_CONTROLLER_BAD 0xFC

enum PS2_CCB {
	PORT_1_IRQ = 0,
	PORT_2_IRQ,
	SYSTEM_FLAG,
	ZERO,
	PORT_1_CLK,
	PORT_2_CLK,
	PORT_1_TRANSL,
	ZERO2
};

enum PS2_STATUS {
	OUT_STATUS = 0,
	IN_STATUS,
	STS_SYSTEM_FLAG,
	CMD_DATA,
	UNKNOWN,
	UNKNOWN2,
	TIMEOUT_ERROR,
	PARITY_ERROR
};

enum PS2_SPECIAL {
	SELF_TEST_GOOD = 0xAA,
	SELF_TEST_BAD = 0xFC,
	SELF_TEST_BAD_2 = 0xFD,
	ACK = 0xFA,
	RESEND = 0xFE
};

static const char scancodes[] = {
	/* letters */
	[0x15] = 'q',
	[0x1d] = 'w',
	[0x24] = 'e',
	[0x2d] = 'r',
	[0x2c] = 't',
	[0x35] = 'y',
	[0x3c] = 'u',
	[0x43] = 'i',
	[0x44] = 'o',
	[0x4d] = 'p',
	[0x1c] = 'a',
	[0x1b] = 's',
	[0x23] = 'd',
	[0x2b] = 'f',
	[0x34] = 'g',
	[0x33] = 'h',
	[0x3b] = 'j',
	[0x42] = 'k',
	[0x4b] = 'l',
	[0x1a] = 'z',
	[0x22] = 'x',
	[0x21] = 'c',
	[0x2a] = 'v',
	[0x32] = 'b',
	[0x31] = 'n',
	[0x3a] = 'm',

	/* numbers */
	[0x45] = '0',
	[0x16] = '1',
	[0x1e] = '2',
	[0x26] = '3',
	[0x25] = '4',
	[0x2e] = '5',
	[0x36] = '6',
	[0x3d] = '7',
	[0x3e] = '8',
	[0x46] = '9',

	/* special */
	[0x29] = ' '
};

extern int ps2_initialize();
extern void ps2_input_wait();
extern void ps2_output_wait();

#endif
