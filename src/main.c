// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

#include "common.h"
#include "timer.h"

int main(struct multiboot *mboot_ptr)
{
    u32int num=124399;
  // All our initialisation calls will go in here.

    desc_tables_init();

    //monitor_clear();
    monitor_write("\n");
    monitor_write_dec(num);
    monitor_write("\n");
    monitor_write_hex(num);
    monitor_write("\n");

    asm volatile ("sti");

    asm volatile ("int $0x3");
    asm volatile ("int $0x4");

    init_timer(50);

    return 0xDEADBEEF;
} 

