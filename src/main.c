/*main.c -- Defines the C-code kernel entry point, calls initialisation routines.
Made for JamesM's tutorials*/

#include "common.h"
#include "timer.h"
#include "multiboot.h"
#include "monitor.h"

/*int main(unsigned long addr)*/
int main(int argc, char *argv[])
{
    int pit_freq = 50;
    /*unsigned long total_memory_size;*/
    multiboot_info_t *mbi;
    /*multiboot_info_t *mbi = (multiboot_info_t*)addr;*/
    if (argc>0)
        mbi = (multiboot_info_t*)argv[1];
    else
        return 0xDEADBEEF;

    /*All our initialisation calls will go in here.*/
    monitor_write("Inside main.c\n");

    monitor_clear();

    /* The following is a little broken....
    The problem is related to how the multiboot info is passed from boot.s.
    In particular, swapping things on and off different stacks, and switching
    paging on.
    This screw-up should be investigated at some stage....
    total_memory_size = 
        ((unsigned long)mbi->mem_lower + 
        (unsigned long)mbi->mem_upper);
    monitor_write("Found ");
    monitor_write_dec((u32int)total_memory_size);
    monitor_write(" kiB of usable RAM.\n");*/

    /*Uncomment the following when paging is set up
    desc_tables_init(); // GDT & IDT
    monitor_write("Global and Interrupt Descriptor Tables (GDT & IDT) configured.\n");
    asm volatile ("int $0x3");
    asm volatile ("int $0x4");*/

    init_timer(pit_freq);
    monitor_write("Programmable Interval Timer firing at ");
    monitor_write_dec(pit_freq);
    monitor_write(" Hz.\n");

    /*Uncomment the following when the IDT is set up
    asm volatile ("sti"); // Restart interrupts
    monitor_write("Interrupts reenabled.\n");*/

    monitor_write("\nBOOTED! :)\n\nWelcome to FuriOS!\n");

    return 0xCAFEBABE;
} 

