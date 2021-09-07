AS = yasm
LD = i686-elf-ld
CC = i686-elf-gcc

KERNEL_OBJ = kernel/entry.o kernel/arch/x86/tty/tty.o kernel/kernel.o

all: boot.img

boot.img: boot kernel/kernel.bin
	cat boot kernel/kernel.bin > $@
	truncate -s1440K $@

boot: boot.s
	$(AS) -f bin boot.s -o $@

kernel/kernel.bin: ${KERNEL_OBJ}
	$(LD) -o $@ -T kernel/linker.ld ${KERNEL_OBJ} --oformat=binary

kernel/entry.o: kernel/entry.s
	$(AS) -f elf kernel/entry.s -o $@

kernel/arch/x86/tty/tty.o: kernel/arch/x86/tty/tty.c
	$(CC) -o $@ -ffreestanding -c kernel/arch/x86/tty/tty.c

kernel/kernel.o: kernel/kernel.c
	$(CC) -o $@ -Iinclude/arch/x86 -ffreestanding -c kernel/kernel.c

clean:
	rm boot kernel/kernel.bin ${KERNEL_OBJ} boot.img

.PHONY: all clean
