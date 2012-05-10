// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

int main(struct multiboot *mboot_ptr)
{
  // All our initialisation calls will go in here.
    monitor_clear();
    monitor_write("Hi there!");
    return 0xDEADBABA;
} 

