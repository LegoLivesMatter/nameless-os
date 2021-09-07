void _start(void)
{
	char *video_memory = (char *) 0xB8000; /* VGA VRAM starts at 0xB8000 */
	*video_memory = 'A'; /* put an A at the beginning of the VRAM */
}
