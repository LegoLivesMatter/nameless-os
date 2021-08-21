AS = yasm # choose yasm or nasm here
ASFLAGS = -f bin # compile a raw binary

boot: boot.s print.s
	$(AS) $(ASFLAGS) -o $@ boot.s

clean:
	-rm boot

run:
	qemu-system-i386 boot

help:
	@echo "Run 'make' to compile."
	@echo "Run 'make clean' to delete compiled objects."
	@echo "Run 'make run' to run the compiled object. Must have qemu-system-i386 (but x86_64 should work too)."

.PHONY = clean run help
