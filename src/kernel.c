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

// TODO: Don't hardcode this
#define MEMLIM 134217728 // 128MiB limit that is QEMU default

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

void terminal_putchar(char c) {
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
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

#define INT2STR_BUFFLEN 11
void int_to_string(unsigned long val, char *buff, size_t len) {
    for (size_t i=0; i<len; i++) buff[i] = 0;

    if (val == 0) {
        buff[0] = '0';
        return;
    }
    unsigned long num_digits = 0;
    unsigned long temp = val;
    while (temp > 0) {
        num_digits++;
        temp /= 10;
    }

    while (val > 0) {
        buff[--num_digits] = (val % 10) + '0';
        val /= 10;
    }
}

void kernel_main(unsigned long magic, unsigned long addr) {
    char int_to_string_buff[INT2STR_BUFFLEN] = {0};

	/* Initialize terminal interface */
	terminal_initialize();

    for (size_t i=0; i<30; i++) {
        for (size_t j=0; j<i; j++) {
            terminal_putchar('*');
        }
        terminal_writestring("Hello world!");
        terminal_putchar('\n');
    }

    terminal_writestring("magic = ");
    int_to_string(magic, int_to_string_buff, sizeof(int_to_string_buff) / sizeof(int_to_string_buff[0]));
    terminal_writestring(int_to_string_buff);
    terminal_putchar('\n');

    terminal_writestring("addr = ");
    int_to_string(addr, int_to_string_buff, sizeof(int_to_string_buff) / sizeof(int_to_string_buff[0]));
    terminal_writestring(int_to_string_buff);
    terminal_putchar('\n');

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        terminal_writestring("Wrong magic\n");
    }
}

