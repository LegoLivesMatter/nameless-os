AS = yasm
LD = i686-elf-ld
CC = i686-elf-gcc

GIT_REV = $(shell git rev-parse --short HEAD)

KERNEL_OBJ = kernel/entry.o kernel/arch/x86/tty/tty.o kernel/drivers/irq/i8259a.o kernel/arch/x86/irq/idt.o kernel/arch/x86/irq/sample_handler.o kernel/kernel.o

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
	$(CC) -std=gnu89 -g -o $@ -Iinclude/arch/x86 -ffreestanding -c kernel/arch/x86/tty/tty.c

kernel/drivers/irq/i8259a.o: kernel/drivers/irq/i8259a.c
	$(CC) -std=gnu89 -g -o $@ -Iinclude/arch/x86 -ffreestanding -c kernel/drivers/irq/i8259a.c

kernel/arch/x86/irq/idt.o: kernel/arch/x86/irq/idt.c
	$(CC) -std=gnu89 -g -o $@ -Iinclude/arch/x86 -ffreestanding -c kernel/arch/x86/irq/idt.c

kernel/arch/x86/irq/sample_handler.o: kernel/arch/x86/irq/sample_handler.c
	$(CC) -std=gnu89 -g -o $@ -Iinclude/arch/x86 -ffreestanding -mgeneral-regs-only -c kernel/arch/x86/irq/sample_handler.c

kernel/kernel.o: kernel/kernel.c
	$(CC) -std=gnu89 -g -o $@ -Iinclude/arch/x86 -ffreestanding -DGIT_COMMIT=\"$(GIT_REV)\" -c kernel/kernel.c

kernel/kernel.elf: kernel/kernel.bin
	$(LD) -o $@ -T kernel/linker.ld ${KERNEL_OBJ} --oformat=elf32-i386

clean:
	rm boot/x86/boot kernel/kernel.bin kernel/kernel.elf ${KERNEL_OBJ} boot.img

.PHONY: all clean
