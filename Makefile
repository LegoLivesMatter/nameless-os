AS = yasm
LD = i686-elf-ld
CC = i686-elf-gcc

all: boot.img

boot.img: boot kernel/kernel.bin
	cat boot kernel/kernel.bin > $@
	truncate -s1440K $@

boot: boot.s
	$(AS) -f bin boot.s -o $@

kernel/kernel.bin: kernel/entry.o kernel/kernel.o
	$(LD) -o $@ -Ttext 0x1000 kernel/entry.o kernel/kernel.o --oformat=binary

kernel/entry.o: kernel/entry.s
	$(AS) -f elf kernel/entry.s -o $@

kernel/kernel.o: kernel/kernel.c
	$(CC) -o $@ -ffreestanding -c kernel/kernel.c

clean:
	rm boot kernel/kernel.bin kernel/entry.o kernel/kernel.o boot.img

.PHONY: all clean
