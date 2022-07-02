#ifndef KPRINT_H
#define KPRINT_H

#include <stdint.h>
#include <tty.h>

extern void tty_initialize();
extern void kprint(const char *string, const enum vga_color fg, const enum vga_color bg);
extern void kprintc(const char character, const enum vga_color fg, const enum vga_color bg);
extern void kprintnibble(uint8_t nibble);
extern void kprintb(const uint8_t byte, const int print_delm);
extern void kprintw(const uint16_t word, const int print_delm);
extern void kprintd(const uint32_t dword, const int print_delm);
extern void kprintq(const uint64_t qword, const int print_delm);
extern int kprintdec(uint32_t num, const enum vga_color fg, const enum vga_color bg);

#endif
