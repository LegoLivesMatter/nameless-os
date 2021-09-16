AS = yasm
LD = i686-elf-ld
CC = i686-elf-gcc

KERNEL_OBJ = kernel/entry.o kernel/arch/x86/tty/tty.o kernel/kernel.o

all: boot.img kernel/kernel.elf

boot.img: boot/x86/boot kernel/kernel.bin
	cat boot/x86/boot kernel/kernel.bin > $@
	truncate -s1440K $@

boot/x86/boot: boot/x86/boot.s boot/x86/a20.s boot/x86/protected.s boot/x86/print.s
	$(AS) -f bin boot/x86/boot.s -o $@

kernel/kernel.bin: ${KERNEL_OBJ}
	$(LD) -o $@ -T kernel/linker.ld ${KERNEL_OBJ} 

kernel/entry.o: kernel/entry.s
	$(AS) -f elf kernel/entry.s -o $@

kernel/arch/x86/tty/tty.o: kernel/arch/x86/tty/tty.c
	$(CC) -g -o $@ -ffreestanding -c kernel/arch/x86/tty/tty.c

kernel/kernel.o: kernel/kernel.c
	$(CC) -g -o $@ -Iinclude/arch/x86 -ffreestanding -c kernel/kernel.c

kernel/kernel.elf: kernel/kernel.bin
	$(LD) -o $@ -T kernel/linker.ld ${KERNEL_OBJ} --oformat=elf32-i386

clean:
	rm boot/x86/boot kernel/kernel.bin ${KERNEL_OBJ} boot.img

.PHONY: all clean
