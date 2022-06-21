default: mbr vbr-fat32 stage3/LOADER.BIN stage3/loader.elf

CFLAGS=-g -ffreestanding
LD=i686-elf-gcc
LDFLAGS=-T stage3/stage3.ld -nostdlib -g

STAGE3_OBJ=stage3/loader.o stage3/paging.o

mbr: mbr.s
	$(AS) $(ASFLAGS) -w-zeroing -o $@ $<

vbr-fat32: vbr-fat32.s fat32/*.s
	$(AS) $(ASFLAGS) -o $@ $<

stage3/loader.elf: $(STAGE3_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ -Wl,--oformat=elf32-i386

stage3/LOADER.BIN: $(STAGE3_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

stage3/loader.o: stage3/loader.s stage3/*.s
	$(AS) $(ASFLAGS) -f elf -g dwarf2 -DGIT_REVISION=\"$(GIT_REV)\" -o $@ $<

clean:
	-rm mbr vbr-fat32 stage3/LOADER.BIN $(STAGE3_OBJ) disk.img

.PHONY : default clean
