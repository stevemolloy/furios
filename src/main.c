// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

int main(struct multiboot *mboot_ptr)
{
  // All our initialisation calls will go in here.
    int i;
    monitor_clear();
    for (i=0; i<strlen("1234"); i++)
    {
        monitor_write("Hi there!\n");
    }
    return 0xDEADBABA;
} 

