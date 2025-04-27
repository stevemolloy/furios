CC = ./build_tools/cross/bin/i686-elf-gcc
AS = ./build_tools/cross/bin/i686-elf-as

all: myos.iso

build/kernel.o: src/kernel.c
	@mkdir -p build
	$(CC) -c src/kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

build/boot.o: src/boot.s
	@mkdir -p build
	$(AS) src/boot.s -o build/boot.o

isodir/boot/myos.bin: build/boot.o build/kernel.o src/linker.ld
	$(CC) -T src/linker.ld -o isodir/boot/myos.bin -ffreestanding -O2 -nostdlib build/boot.o build/kernel.o -lgcc

myos.iso: isodir/boot/myos.bin isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

run: myos.iso
	qemu-system-i386 myos.iso

clean:
	rm -rf build
	rm -rf myos.iso
	rm -rf isodir/boot/myos.bin

