// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

int main(struct multiboot *mboot_ptr)
{
    int i;
  // All our initialisation calls will go in here.

    desc_tables_init();

    //monitor_clear();
    for (i=0; i<strlen("12345678"); i++)
    {
        monitor_write("\nHi there!");
    }

    return 0xDEADBEEF;
} 

