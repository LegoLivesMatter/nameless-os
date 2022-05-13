export AS = yasm
export CC = i686-elf-gcc
QEMU = qemu-system-i386 -monitor stdio

export GIT_REV = $(shell git describe --long HEAD)

CFLAGS = -std=gnu89 -g -Iinclude/arch/x86 -ffreestanding -DGIT_COMMIT=\"$(GIT_REV)\"

KERNEL_OBJ = kernel/entry.o kernel/arch/x86/tty/tty.o kernel/drivers/irq/i8259a.o kernel/arch/x86/irq/idt.o kernel/arch/x86/irq/sample_handler.o kernel/kernel.o
BOOTLOADER_OBJ = boot/x86/mbr boot/x86/vbr-fat32 boot/x86/stage3/LOADER.BIN

default: kernel/kernel.elf bootloader

all: default boot/x86/disk.img

bootloader: $(BOOTLOADER_OBJ)

run: all
	$(QEMU) boot/x86/disk.img

boot/x86/mbr: boot/x86/mbr.s
boot/x86/vbr-fat32: boot/x86/vbr-fat32.s boot/x86/fat32/*.s 
boot/x86/stage3/LOADER.BIN: boot/x86/stage3/*.s boot/x86/fat32/*.s
$(BOOTLOADER_OBJ):
	$(MAKE) -C boot/x86

boot/x86/disk.img: boot/x86/mbr boot/x86/vbr-fat32 boot/x86/stage3/LOADER.BIN boot/x86/disk.dump kernel/kernel.bin
	truncate -s1G boot/x86/disk.img
	sfdisk boot/x86/disk.img < boot/x86/disk.dump
	mkfs.fat -F 32 --offset 2048 boot/x86/disk.img
	dd if=boot/x86/mbr of=boot/x86/disk.img bs=440 count=1 conv=notrunc
	dd if=boot/x86/vbr-fat32 of=boot/x86/disk.img bs=1 skip=90 seek=1048666 conv=notrunc
	mcopy -i boot/x86/disk.img@@1M boot/x86/stage3/LOADER.BIN ::.
	mcopy -i boot/x86/disk.img@@1M kernel/kernel.bin ::./KERNEL.BIN

kernel/kernel.bin: ${KERNEL_OBJ} kernel/linker.ld
	$(CC) -ffreestanding -nostdlib -o $@ -T kernel/linker.ld ${KERNEL_OBJ}

kernel/entry.o: kernel/entry.s
	$(AS) -f elf $< -o $@

kernel/arch/x86/irq/sample_handler.o: kernel/arch/x86/irq/sample_handler.c
	$(CC) $(CFLAGS) -mgeneral-regs-only -c $< -o $@

kernel/kernel.elf: kernel/kernel.bin
	$(CC) -ffreestanding -nostdlib -o $@ -T kernel/linker.ld ${KERNEL_OBJ} -Wl,--oformat=elf32-i386
	i686-elf-objcopy --only-keep-debug kernel/kernel.elf kernel/kernel.dbg
	i686-elf-objcopy --add-gnu-debuglink=kernel/kernel.dbg kernel/kernel.elf
	i686-elf-strip --strip-unneeded kernel/kernel.elf

clean:
	-rm kernel/kernel.{bin,dbg,elf} ${KERNEL_OBJ}
	cd boot/x86 && $(MAKE) clean

.PHONY: default all clean run bootloader
