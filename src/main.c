// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

#include "common.h"

int main(struct multiboot *mboot_ptr)
{
    int i;
    u32int num=124399;
  // All our initialisation calls will go in here.

    desc_tables_init();

    //monitor_clear();
    for (i=0; i<strlen("12345678"); i++)
    {
        monitor_write("\nHi there!    ");
        monitor_write_dec(i);
    }
    monitor_write("\n");
    monitor_write_dec(num);
    monitor_write("\n");
    monitor_write_hex(num);

    return 0xDEADBEEF;
} 

