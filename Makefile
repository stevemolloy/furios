AS=i686-elf-as
CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra

build: furios.bin
	mkdir -p isodir/boot/grub
	cp furios.bin isodir/boot/furios.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o furios.iso isodir
	rm -rf isodir

furios.bin: boot.o kernel.o
	$(CC) -T linker.ld -o furios.bin $(CFLAGS) -nostdlib boot.o kernel.o -lgcc

boot.o: boot.s
	$(AS) boot.s -o boot.o

kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o $(CFLAGS)

clean:
	rm -f *.o furios.bin

