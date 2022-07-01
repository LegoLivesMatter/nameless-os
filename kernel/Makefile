KERNEL_OBJ = kernel.o entry.o

include drivers/Makefile
include arch/x86/Makefile

ASFLAGS_BASE = -f elf -g dwarf2
CFLAGS_BASE = -fgnu89-inline -ffreestanding -nostdlib -Iinclude \
	      -Iinclude/arch/x86 -g -DGIT_COMMIT=\"$(GIT_REV)\" \
	      -O2 -flto
LDFLAGS_BASE = -ffreestanding -nostdlib -lgcc -g -flto

kernel.elf kernel.dbg: $(KERNEL_OBJ)
	$(REAL_CC) $(LDFLAGS_BASE) $(LDFLAGS) -T linker.ld $^ -o $@
	$(CROSS_COMPILE)objcopy --only-keep-debug kernel.elf kernel.dbg
	$(CROSS_COMPILE)objcopy --add-gnu-debuglink=kernel.dbg kernel.elf
	$(CROSS_COMPILE)strip --strip-unneeded kernel.elf

clean:
	-rm $(KERNEL_OBJ) kernel.elf kernel.dbg

# Implicit rules
%.o: %.c
	$(REAL_CC) $(CFLAGS_BASE) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS_BASE) $(ASFLAGS) $< -o $@

.PHONY: clean
