STAGE3_OBJ = loader.o elf.o paging.o prekernel.o

ASFLAGS_BASE = -f elf -g dwarf2 -DGIT_REVISION=\"$(GIT_REV)\"
CFLAGS_BASE = -ffreestanding -nostdlib -DGIT_REVISION=\"$(GIT_REV)\" -g \
	      -O2 -flto
LDFLAGS_BASE = -ffreestanding -nostdlib -g -lgcc -T stage3.ld -flto

default: LOADER.BIN loader.elf

loader.elf: $(STAGE3_OBJ)
	$(CC) $(LDFLAGS_BASE) $(LDFLAGS) -Wl,--oformat=elf32-i386 $^ -o $@

LOADER.BIN: $(STAGE3_OBJ)
	$(CC) $(LDFLAGS_BASE) $(LDFLAGS) $^ -o $@

# Implicit rules
%.o: %.c
	$(CC) $(CFLAGS_BASE) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS_BASE) $(ASFLAGS) $< -o $@

clean:
	-rm -f $(STAGE3_OBJ) LOADER.BIN loader.elf

.PHONY: default clean
