#ifndef TTY_H
#define TTY_H

#include <stdint.h>

#define VGA_COLOR_BLACK 0x0
#define VGA_COLOR_BLUE 0x1
#define VGA_COLOR_GREEN 0x2
#define VGA_COLOR_TEAL 0x3
#define VGA_COLOR_DARK_RED 0x4
#define VGA_COLOR_MAGENTA 0x5
#define VGA_COLOR_BROWN 0x6
#define VGA_COLOR_LIGHT_GRAY 0x7
#define VGA_COLOR_DARK_GRAY 0x8
#define VGA_COLOR_PURPLE 0x9
#define VGA_COLOR_LIME 0xA
#define VGA_COLOR_CYAN 0xB
#define VGA_COLOR_BRIGHT_RED 0xC
#define VGA_COLOR_PINK 0xD
#define VGA_COLOR_YELLOW 0xE
#define VGA_COLOR_WHITE 0xF

extern void screen_clear(void);
extern void kprint(const char *string, uint8_t color);
extern void kprintc(const char character, uint8_t color);
extern void kprintb(uint8_t byte, int print_delm);
extern void kprintw(uint16_t word, int print_delm);
extern void kprintd(uint32_t dword, int print_delm);
extern void kprintq(uint64_t qword, int print_delm);
extern int kprintdec(uint32_t num, uint8_t color);
#endif
