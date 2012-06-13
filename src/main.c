/*main.c -- Defines the C-code kernel entry point, calls initialisation routines.
Made for JamesM's tutorials*/

#include "common.h"
#include "timer.h"
#include "multiboot.h"
#include "monitor.h"
#include "desc_tables.h"
#include "physmemmanag.h"
#include "pagemanager.h"

extern const void end; /* The symbol for the end of the kernel memory*/
u32int *physstack_start; /* These three lines define the physical memory...*/
u32int *physstack_end; /* ... */
u32int *physstack_ptr; /* ...manager stack*/

int main(int argc, char **argv)
{
	int pit_freq = 50;
	unsigned long total_memory_size;
	multiboot_info_t *mbi;
	u32int kernelend;
	u32int page_dir;
	unsigned long addr;

	kernelend = (u32int)&end - 0xC0000000; /* Kernel is linked at 0xC0000000*/

	if(argc<2){
		return 0xDEADBEEF;
	}

	addr = argv[1];
	mbi = (multiboot_info_t*)addr;
	page_dir = argv[2];

	/*Initialise the screen (clear it!)*/
	monitor_clear();

	desc_tables_init(); /*GDT & IDT*/
	monitor_write("Global and Interrupt Descriptor Tables (GDT & IDT) configured.\n");
	/*asm volatile ("int $0x3");
    asm volatile ("int $0x4");*/

	init_timer(pit_freq);
	monitor_write("Programmable Interval Timer firing at ");
	monitor_write_dec(pit_freq);
	monitor_write(" Hz.\n");

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

	/*physmemstack_init((u32int)total_memory_size, kernelend);*/
	physmemstack_init(mbi, kernelend);
	monitor_write("Physical memory stack initialised & filled\n");
	monitor_write("Pointer to end of physical memory stack = 0x");
	monitor_write_hex((u32int)physstack_end);
	monitor_write("\n");

	/* Initialise the page manager (virtual mem management)*/
	page_manager_init(page_dir);
	monitor_write("Identity mapping of first 4MB (physical) has been removed.\n");
	monitor_write("        Kernel now purely higher half (virtually, not physically!)\n");

	/* Restart interrupts*/
	asm volatile ("sti");
	monitor_write("Interrupts reenabled.\n");

	monitor_write("\nBOOTED! :)\n\nWelcome to FuriOS!\n");

	return 0xCAFEBABE;
} 

