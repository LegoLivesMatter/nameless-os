default: mbr vbr-fat32 stage3/LOADER.BIN

LD=i686-elf-gcc
LDFLAGS=-T stage3/stage3.ld -nostdlib

STAGE3_OBJ=stage3/loader.o

mbr: mbr.s
	$(AS) $(ASFLAGS) -w-zeroing -o $@ $<

vbr-fat32: vbr-fat32.s fat32/*.s
	$(AS) $(ASFLAGS) -o $@ $<

stage3/LOADER.BIN: $(STAGE3_OBJ)
	$(LD) $(LDFLAGS) -o $@ $<

stage3/loader.o: stage3/loader.s stage3/*.s
	$(AS) $(ASFLAGS) -f elf -g dwarf2 -DGIT_REVISION=\"$(GIT_REV)\" -o $@ $<

clean:
	-rm mbr vbr-fat32 stage3/LOADER.BIN $(STAGE3_OBJ) disk.img

.PHONY : default clean
