# Automatically generate lists of sources using wildcards.
C_SOURCES = $(wildcard $(KERNELDIR)/*.c lib/*/*.c $(KERNELDIR)/drivers/*.c)
CPP_SOURCES = $(wildcard $(KERNELDIR)/*.cpp lib/*/*.cpp $(KERNELDIR)/drivers/*.cpp)
ASM_SOURCES = $(KERNELDIR)/utility.asm
HEADERS = $(wildcard $(KERNELDIR)/*.h)
CC = ~/i686-elf/bin/i386-unknown-elf-gcc

BUILDDIR = build
KERNELDIR = kernel

CFLAGS = -nostdlib -ffreestanding -g -Wall -Wextra

OBJS = ${C_SOURCES:.c=.o}

# Default build target
all: build
runboot: all
	qemu-system-i386 -m 32M -serial stdio -soundhw sb16,pcspk -fda os.iso
# Run the operating system on qemu
run: all
	qemu-system-i386 -m 32M -serial stdio -soundhw sb16,pcspk -kernel $(BUILDDIR)/kernel.bin

monitor: all
	qemu-system-i386 -m 32M -monitor stdio -kernel $(BUILDDIR)/kernel.bin

debug: all
	qemu-system-i386 -m 32M -s -S -monitor stdio -kernel $(BUILDDIR)/kernel.bin

build: asm_objects linker.ld makeboot bootloader
	$(CC) -g -I lib/includes -I kernel/includes  $(CPP_SOURCES) $(C_SOURCES) $(BUILDDIR)/*.o -o $(BUILDDIR)/kernel.bin -nostdlib -ffreestanding -static-libgcc -lgcc -T linker.ld
	./makebootable os.img $(BUILDDIR)/boot.bin $(BUILDDIR)/kernel.bin
asm_objects:
	mkdir -p build
	nasm -f elf32 $(KERNELDIR)/boot.asm -o $(BUILDDIR)/boot.o
	nasm -f elf32 $(KERNELDIR)/utility.asm -o $(BUILDDIR)/utility.o
	nasm -f elf32 $(KERNELDIR)/interrupts.asm -o $(BUILDDIR)/interrupts.o 
	nasm -f elf32 $(KERNELDIR)/logger.asm -o $(BUILDDIR)/logger.o
	nasm -f elf32 $(KERNELDIR)/fallback.asm -o $(BUILDDIR)/fallback.o
	nasm -f elf $(KERNELDIR)/shutdown.asm -o $(BUILDDIR)/shutdown.o
bootloader:
	nasm -f bin $(KERNELDIR)/bootloader.asm -o $(BUILDDIR)/boot.bin
makeboot:
	g++ utility/makebootable.c -o makebootable
clean:
	rm -rf $(BUILDDIR)/*
