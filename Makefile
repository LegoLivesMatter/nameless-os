export AS = yasm
export CC = i686-elf-gcc
export QEMU = qemu-system-i386 -monitor stdio
export GIT_REV = $(shell git describe --long HEAD)
MAKEFLAGS += -rR

default: kernel/kernel.elf bootloader
all: default boot/x86/disk.img
run: all
	$(QEMU) boot/x86/disk.img
debug: all
	$(QEMU) -s -S boot/x86/disk.img

kernel/kernel.elf:
	$(MAKE) -C kernel
bootloader:
	$(MAKE) -C boot/x86

boot/x86/disk.img: bootloader boot/x86/disk.dump kernel/kernel.elf
	truncate -s1G boot/x86/disk.img
	sfdisk boot/x86/disk.img < boot/x86/disk.dump
	mkfs.fat -F 32 --offset 2048 boot/x86/disk.img
	dd if=boot/x86/mbr of=boot/x86/disk.img bs=440 count=1 conv=notrunc
	dd if=boot/x86/vbr-fat32 of=boot/x86/disk.img bs=1 skip=90 seek=1048666 conv=notrunc
	mcopy -i boot/x86/disk.img@@1M boot/x86/stage3/LOADER.BIN ::.
	mcopy -i boot/x86/disk.img@@1M kernel/kernel.elf ::./KERNEL.ELF

clean:
	$(MAKE) -C boot/x86 clean
	$(MAKE) -C kernel clean

# Even though kernel.elf is a real target, it's considered phony so that the
# kernel Makefile is always run. We don't check is the kernel binary up-to-date
# here because we want to be more recursive.
.PHONY: default all clean run debug bootloader kernel/kernel.elf
