export AS = yasm
export LD = i686-elf-ld
export CC = i686-elf-gcc
QEMU = qemu-system-i386

GIT_REV = $(shell git rev-parse --short HEAD)

CFLAGS = -std=gnu89 -g -Iinclude/arch/x86 -ffreestanding -DGIT_COMMIT=\"$(GIT_REV)\"

KERNEL_OBJ = kernel/entry.o kernel/arch/x86/tty/tty.o kernel/drivers/irq/i8259a.o kernel/arch/x86/irq/idt.o kernel/arch/x86/irq/sample_handler.o kernel/kernel.o

default: kernel/kernel.elf

all: default boot/x86/disk.img

run: boot/x86/disk.img
	$(QEMU) boot/x86/disk.img

boot/x86/disk.img: boot/x86/mbr.s boot/x86/vbr-fat32.s boot/x86/loader.s boot/x86/disk.dump boot/x86/fat32.s boot/x86/fat32-structs.s
	cd boot/x86 && $(MAKE) all

kernel/kernel.bin: ${KERNEL_OBJ}
	$(LD) -o $@ -T kernel/linker.ld ${KERNEL_OBJ}

kernel/entry.o: kernel/entry.s
	$(AS) -f elf kernel/entry.s -o $@

kernel/arch/x86/tty/tty.o: kernel/arch/x86/tty/tty.c

kernel/drivers/irq/i8259a.o: kernel/drivers/irq/i8259a.c

kernel/arch/x86/irq/idt.o: kernel/arch/x86/irq/idt.c

kernel/arch/x86/irq/sample_handler.o: kernel/arch/x86/irq/sample_handler.c
	$(CC) $(CFLAGS) -mgeneral-regs-only -c kernel/arch/x86/irq/sample_handler.c -o $@

kernel/kernel.o: kernel/kernel.c

kernel/kernel.elf: kernel/kernel.bin
	$(LD) -o $@ -T kernel/linker.ld ${KERNEL_OBJ} --oformat=elf32-i386

clean:
	-rm kernel/kernel.bin kernel/kernel.elf ${KERNEL_OBJ}
	cd boot/x86 && $(MAKE) clean

.PHONY: default all clean run
