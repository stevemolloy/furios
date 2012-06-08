/*main.c -- Defines the C-code kernel entry point, calls initialisation routines.
Made for JamesM's tutorials*/

#include "common.h"
#include "timer.h"
#include "multiboot.h"
#include "monitor.h"
#include "desc_tables.h"
#include "physmemmanag.h"

extern const void end; /* The symbol for the end of the kernel memory*/
u32int *physstack_end; /* These two lines define the physical memory...*/
u32int *physstack_ptr; /* ...manager stack*/

int main(unsigned long addr, u32int page_dir)
{
    int pit_freq = 50;
    unsigned long total_memory_size;
    multiboot_info_t *mbi;
    u32int kernelend;

    kernelend = (u32int)&end - 0xC0000000; /* Kernel is linked at 0xC0000000*/

    mbi = (multiboot_info_t*)addr;

    /*Initialise the screen (clear it!)*/
    monitor_clear();

    /* Page dir location stored in page_dir*/
    monitor_write("Page directory at 0x");
    monitor_write_hex(page_dir);
    monitor_write("\n");

    monitor_write("Kernel end symbol at 0x");
    monitor_write_hex(kernelend);
    monitor_write("\n");

    /* Get the memory map*/
    total_memory_size = 
        ((unsigned long)mbi->mem_lower + 
        (unsigned long)mbi->mem_upper);
    monitor_write("Found ");
    monitor_write_dec((u32int)total_memory_size);
    monitor_write(" kiB of usable RAM.\n");

    physmemstack_init((u32int)total_memory_size, kernelend);
    monitor_write("Physical memory stack initialised & filled\n");
    monitor_write("Pointer to end of physical memory stack = 0x");
    monitor_write_hex((u32int)physstack_end);
    monitor_write("\n");

    desc_tables_init(); /*IDT*/
    monitor_write("Global and Interrupt Descriptor Tables (GDT & IDT) configured.\n");
    asm volatile ("int $0x3");
    asm volatile ("int $0x4");

    init_timer(pit_freq);
    monitor_write("Programmable Interval Timer firing at ");
    monitor_write_dec(pit_freq);
    monitor_write(" Hz.\n");

    asm volatile ("sti"); /* Restart interrupts*/
    monitor_write("Interrupts reenabled.\n");

    monitor_write("\nBOOTED! :)\n\nWelcome to FuriOS!\n");

    return 0xCAFEBABE;
} 

