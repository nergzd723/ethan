# Automatically generate lists of sources using wildcards.
C_SOURCES = $(wildcard $(KERNELDIR)/*.c lib/*/*.c $(KERNELDIR)/drivers/*.c)
CPP_SOURCES = $(wildcard $(KERNELDIR)/*.cpp lib/*/*.cpp $(KERNELDIR)/drivers/*.cpp)
ASM_SOURCES = $(KERNELDIR)/utility.asm 
HEADERS = $(wildcard $(KERNELDIR)/*.h)
CC = ~/i686-elf/bin/i386-unknown-elf-gcc
BUILDDIR = build
KERNELDIR = kernel
BOOTDIR = $(KERNELDIR)/boot
CFLAGS = -nostdlib -ffreestanding -g -Wall -Wextra

OBJS = ${C_SOURCES:.c=.o}

# Default build target
all: build userapp grub
userapp: userlinker.ld asm_objects
	$(CC) -I lib/includes -g -nostdlib -ffreestanding -static-libgcc -lgcc -o build/userapp.bin userapp/userapp.c build/usr.o userapp/build/usrldr.o -T userlinker.ld
	cp build/userapp.bin isodir/boot/app.bin
runboot: all
	qemu-system-i386 -m 32M -serial stdio -soundhw sb16,pcspk -fda os.iso
# Run the operating system on qemu
run: all
	qemu-system-i386 -m 32M -serial stdio -soundhw sb16,pcspk os.iso

monitor: all
	qemu-system-i386 -m 32M -monitor stdio -kernel $(BUILDDIR)/kernel.bin

debug: all
	qemu-system-i386 -m 32M -s -S -monitor stdio -kernel $(BUILDDIR)/kernel.bin

build: asm_objects linker.ld userapp
	$(CC) -g -I lib/includes -I kernel/includes  $(CPP_SOURCES) $(C_SOURCES) $(BUILDDIR)/*.o -o $(BUILDDIR)/kernel.bin -nostdlib -ffreestanding -static-libgcc -lgcc -T linker.ld
	#./makebootable os.img $(BUILDDIR)/boot.bin $(BUILDDIR)/kernel.bin
grub:
	cp $(BUILDDIR)/kernel.bin isodir/boot/kernel.bin
	grub-mkrescue isodir -o os.iso
asm_objects:
	mkdir -p build
	nasm -f elf $(BOOTDIR)/boot.asm -o $(BUILDDIR)/boot.o
	nasm -f elf32 $(KERNELDIR)/utility.asm -o $(BUILDDIR)/utility.o
	nasm -f elf32 $(KERNELDIR)/interrupts.asm -o $(BUILDDIR)/interrupts.o 
	nasm -f elf32 $(KERNELDIR)/logger.asm -o $(BUILDDIR)/logger.o
	nasm -f elf32 $(KERNELDIR)/fallback.asm -o $(BUILDDIR)/fallback.o
	nasm -f elf $(KERNELDIR)/shutdown.asm -o $(BUILDDIR)/shutdown.o
	nasm -f elf32 lib/ethlib/user.asm -o usr.o
	nasm -f elf32 userapp/usrldr.asm -o userapp/build/usrldr.o
bootloader:
	nasm -f bin $(BOOTDIR)/bootloader.asm -o $(BUILDDIR)/boot.bin
makeboot:
	g++ utility/makebootable.c -o makebootable
clean:
	rm -rf $(BUILDDIR)/*
