all:
	nasm -fbin methanium.asm -o boot.bin
	nasm -fbin ethaniumjr.asm -o KERNEL.BIN
	mformat -f1440 -B boot.bin -C -i disk.img
	mcopy -D o -i disk.img KERNEL.BIN ::/
	qemu-system-i386 -serial stdio -fda disk.img