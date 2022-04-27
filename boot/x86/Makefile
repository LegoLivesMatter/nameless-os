default: mbr vbr-fat32 LOADER.BIN

all: mbr vbr-fat32 LOADER.BIN disk.img

mbr: mbr.s
	$(AS) $(ASFLAGS) -w-zeroing -o $@ $<

vbr-fat32: vbr-fat32.s fat32.s fat32-structs.s
	$(AS) $(ASFLAGS) -o $@ $<

LOADER.BIN: loader.s
	$(AS) $(ASFLAGS) -o $@ $<

disk.img: mbr vbr-fat32 LOADER.BIN disk.dump
	truncate -s100M disk.img
	sfdisk disk.img < disk.dump
	mkfs.fat -F 32 --offset 2048 disk.img
	dd if=mbr of=disk.img bs=440 count=1 conv=notrunc
	dd if=vbr-fat32 of=disk.img bs=1 skip=90 seek=1048666 conv=notrunc
	mcopy -i disk.img@@1M LOADER.BIN ::.

clean:
	-rm mbr vbr-fat32 LOADER.BIN disk.img

.PHONY : default all clean
