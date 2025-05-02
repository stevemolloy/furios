#include "multiboot.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
typedef enum {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} VGA_COLOR;

#define ARRLEN(arr) sizeof((arr)) / sizeof((arr)[0])
#define INT2STR_BUFFLEN 11
char int_to_string_buff[INT2STR_BUFFLEN] = {0};

static inline uint8_t vga_entry_color(VGA_COLOR fg, VGA_COLOR bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xC03FF000;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll_up(void) {
    for (size_t y=0; y<VGA_HEIGHT-1; y++) {
        for (size_t x=0; x<VGA_WIDTH; x++) {
            terminal_buffer[y*VGA_WIDTH + x] = terminal_buffer[(y+1)*VGA_WIDTH + x];
        }
    }
    for (size_t x=0; x<VGA_WIDTH; x++) {
        terminal_buffer[(VGA_HEIGHT-1)*VGA_WIDTH + x] = ' ';
    }
}

void kputchar(char c) {
	if (terminal_row == VGA_HEIGHT) {
        terminal_scroll_up();
        terminal_row--;
    }
    if (c >= 0x20 && c < 0x7F) {
	    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    }
	if (c == '\n' || ++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
        terminal_row++;
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		kputchar(data[i]);
}

void kprint_str(const char* data) {
	terminal_write(data, strlen(data));
}

void int_to_string(uint32_t val, char *buff, size_t len, uint32_t base) {
    for (size_t i=0; i<len; i++) buff[i] = 0;

    if (val == 0) {
        buff[0] = '0';
        return;
    }
    uint32_t num_digits = 0;
    uint32_t temp = val;
    while (temp > 0) {
        num_digits++;
        temp /= base;
    }

    while (val > 0) {
        uint32_t buff_val = (val % base);
        if (buff_val > 9) {
            buff[--num_digits] = (buff_val - 10) + 'A';
        } else {
            buff[--num_digits] = (val % base) + '0';
        }
        val /= base;
    }
}

void kprint_int(uint32_t val, uint32_t base) {
    int_to_string(val, int_to_string_buff, ARRLEN(int_to_string_buff), base);
    kprint_str(int_to_string_buff);
}

void kernel_main(uint32_t magic, uint32_t physaddr) {
	/* Initialize terminal interface */
	terminal_initialize();

    kprint_str("Welcome to FuriOS\n");
    kprint_str("=================\n");

    kprint_str("Magic = 0x");
    kprint_int(magic, 16);
    kprint_str(" (should be 0x");
    kprint_int(MULTIBOOT_BOOTLOADER_MAGIC, 16);
    kprint_str(")\n");

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        kprint_str("Wrong magic (see above)\n");
        return;
    }

    kprint_str("Physaddr of multiboot header = 0x");
    kprint_int(physaddr, 16);
    kputchar('\n');

    multiboot_info_t *addr = (multiboot_info_t*)(physaddr + 0xC0000000);

    kprint_str("multiboot flags = 0b");
    kprint_int(addr->flags, 2);
    kputchar('\n');

    kprint_str("multiboot mem_upper = ");
    kprint_int(addr->mem_upper, 10);
    kputchar('\n');

    /* Check bit 6 to see if we have a valid memory map */
    if(!(addr->flags >> 6 & 0x1)) {
        kprint_str("invalid memory map given by GRUB bootloader");
        return;
    }

    /* Loop through the memory map and display the values */
    kprint_str("\n");
    kprint_str("Memory map provided by GRUB:\n");
    for(int i = 0; i < (int)addr->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*)(addr->mmap_addr + 0xC0000000 + i);

        kprint_str("    Start: 0x");
        kprint_int(mmmt->addr, 16);
        kprint_str(" | Length: 0x");
        kprint_int(mmmt->len, 16);
        kprint_str(" | Type: ");
        kprint_int(mmmt->type, 10);
        kprint_str("\n");

        // printf("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
        //     mmmt->addr, mmmt->len, mmmt->size, mmmt->type);

        // if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
        //     /* 
        //      * Do something with this memory block!
        //      * BE WARNED that some of memory shown as availiable is actually 
        //      * actively being used by the kernel! You'll need to take that
        //      * into account before writing to memory!
        //      */
        // }
    }
}

