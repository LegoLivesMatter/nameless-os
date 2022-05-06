default: mbr vbr-fat32 stage3/LOADER.BIN

mbr: mbr.s
	$(AS) $(ASFLAGS) -w-zeroing -o $@ $<

vbr-fat32: vbr-fat32.s fat32/*.s
	$(AS) $(ASFLAGS) -o $@ $<

stage3/LOADER.BIN: stage3/loader.s stage3/*.s
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	-rm mbr vbr-fat32 stage3/LOADER.BIN disk.img

.PHONY : default clean
