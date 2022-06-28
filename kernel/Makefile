KERNEL_OBJ = kernel.o entry.o

include drivers/Makefile
include arch/x86/Makefile

ASFLAGS_BASE = -f elf -g dwarf2
CFLAGS_BASE = -fgnu89-inline -ffreestanding -nostdlib -Iinclude -Iinclude/arch/x86 -g -DGIT_COMMIT=\"$(GIT_REV)\"
LDFLAGS_BASE = -ffreestanding -nostdlib -lgcc -g

kernel.elf kernel.dbg: $(KERNEL_OBJ)
	$(CC) $(LDFLAGS_BASE) $(LDFLAGS) -T linker.ld $^ -o $@
	i686-elf-objcopy --only-keep-debug kernel.elf kernel.dbg
	i686-elf-objcopy --add-gnu-debuglink=kernel.dbg kernel.elf
	i686-elf-strip --strip-unneeded kernel.elf

clean:
	-rm $(KERNEL_OBJ) kernel.elf kernel.dbg

# Implicit rules
%.o: %.c
	$(CC) $(CFLAGS_BASE) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS_BASE) $(ASFLAGS) $< -o $@

.PHONY: clean
