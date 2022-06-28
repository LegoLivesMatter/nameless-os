BOOTLOADER_OBJ = mbr vbr-fat32
ASFLAGS_BASE = -w-zeroing

default: $(BOOTLOADER_OBJ) stage3

stage3:
	$(MAKE) -C stage3

vbr-fat32: vbr-fat32.s fat32/*.s

clean:
	-rm mbr vbr-fat32 disk.img
	$(MAKE) -C stage3 clean

# Implicit rules
%: %.s
	$(AS) $(ASFLAGS_BASE) -f bin $< -o $@

.PHONY : default clean stage3
