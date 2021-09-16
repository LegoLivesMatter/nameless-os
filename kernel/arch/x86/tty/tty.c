#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile char *video_memory = (char *) 0xB8000; /* VGA VRAM starts at 0xB8000 */

static int cursor_x = 0;
static int cursor_y = 0;

void screen_clear(void)
{
	int x, y;
	for ( y = 0; y < VGA_HEIGHT; y++ ) {
		for ( x = 0; x < VGA_WIDTH; x++ ) {
			video_memory[(y * VGA_WIDTH + x) * 2 + 1] = 0x07;
			video_memory[(y * VGA_WIDTH + x) * 2] = ' ';
		}
	}

	cursor_x = 0;
	cursor_y = 0;
}

void scroll_up(void)
{
	int x, y;
	for ( y = 1; y < VGA_HEIGHT; y++ ) {
		for ( x = 0; x < VGA_WIDTH; x++ ) {
			video_memory[((y - 1) * VGA_WIDTH + x) * 2] = video_memory[(y * VGA_WIDTH + x) * 2];
		}
	}
	for ( x = 0; x < VGA_WIDTH; x++ ) {
		video_memory[((VGA_HEIGHT - 1) * VGA_WIDTH + x) * 2] = ' ';
	}
	cursor_y = VGA_HEIGHT - 1;
}

void kprint(const char *string)
{
	char next_char;
	next_char = *string;

	while ( next_char != 0 ) {
		if ( next_char == '\n') { cursor_x = 0; cursor_y++; }
		else { video_memory[(cursor_y * VGA_WIDTH + cursor_x++) * 2] = next_char; }
		if ( cursor_x >= VGA_WIDTH ) { cursor_x = 0; cursor_y++; }
		if ( cursor_y >= VGA_HEIGHT ) { scroll_up(); }
		next_char = *++string;
	}
}
