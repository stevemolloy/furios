/*main.c -- Defines the C-code kernel entry point, calls initialisation routines.
Made for JamesM's tutorials*/

#include "common.h"
#include "timer.h"
#include "multiboot.h"
#include "monitor.h"
#include "desc_tables.h"

/*int main(int argc, char **argv)*/
int main(unsigned long addr, u32int page_dir)
{
    int pit_freq = 50;
    unsigned long total_memory_size;
    multiboot_info_t *mbi = (multiboot_info_t*)addr;
    /*u32int page_dir;

    if (argc>0)
    {
        mbi = (multiboot_info_t*)argv[2];
        page_dir = argv[0];
    }
    else
        return 0xDEADBEEF;*/

    monitor_clear();

    monitor_write("Page directory at 0x");
    /*monitor_write_hex(*page_dir);*/
    monitor_write_hex(page_dir);
    monitor_write("\n");

    /* The following is a little broken....
    The problem is related to how the multiboot info is passed from boot.s.
    In particular, swapping things on and off different stacks, and switching
    paging on.
    This screw-up should be investigated at some stage....*/
    total_memory_size = 
        ((unsigned long)mbi->mem_lower + 
        (unsigned long)mbi->mem_upper);
    monitor_write("Found ");
    monitor_write_dec((u32int)total_memory_size);
    monitor_write(" kiB of usable RAM.\n");

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

