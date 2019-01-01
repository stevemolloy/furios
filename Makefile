AS=i686-elf-as
CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra

furios.bin: boot.o kernel.o
	$(CC) -T linker.ld -o furios.bin $(CFLAGS) -nostdlib boot.o kernel.o -lgcc

boot.o: boot.s
	$(AS) boot.s -o boot.o

kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o $(CFLAGS)

clean:
	rm *.o furios.bin

