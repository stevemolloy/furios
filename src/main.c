// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

#include "common.h"
#include "timer.h"
#include "multiboot.h"

//typedef struct multiboot_memory_map {
//	unsigned int size;
//	unsigned int base_addr_low,base_addr_high;
//// You can also use: unsigned long long int base_addr; if supported.
//	unsigned int length_low,length_high;
//// You can also use: unsigned long long int length; if supported.
//	unsigned int type;
//        unsigned int mem_lower, mem_upper;
//} multiboot_memory_map_t;
//
//int main(multiboot_memory_map_t *mboot_ptr)
int main(unsigned long addr)
{
    // All our initialisation calls will go in here.
    int pit_freq = 50;

    monitor_clear();

    multiboot_info_t *mbi = (multiboot_info_t*)addr;
    unsigned long total_memory_size = 
        ((unsigned long)mbi->mem_lower + 
        (unsigned long)mbi->mem_upper);
    monitor_write("Found ");
    monitor_write_dec((u32int)total_memory_size);
    monitor_write(" kiB of usable RAM.\n");

    desc_tables_init(); // GDT & IDT
    monitor_write("Global and Interrupt Descriptor Tables (GDT & IDT) configured.\n");

    asm volatile ("sti"); // Restart interrupts
    monitor_write("Interrupts reenabled.\n");

    //asm volatile ("int $0x3");
    //asm volatile ("int $0x4");

    init_timer(pit_freq);
    monitor_write("Programmable Interval Timer firing at ");
    monitor_write_dec(pit_freq);
    monitor_write(" Hz.\n");

    monitor_write("\nBOOTED! :)\n\nWelcome to FuriOS!\n");

    return 0xDEADBEEF;
} 

