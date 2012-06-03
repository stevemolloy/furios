/* Link.ld -- Linker script for the kernel - ensure everything goes in the */
/*            Correct place.  */
/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */

ENTRY(start)
SECTIONS
{
  . = 0x100000;

  .setup :
  {
    *(.setup)
  }

  . = ALIGN(4096);
  /*offset = .;*/
  . += 0xC0000000;

  /*.text : AT(offset)*/
  .text : AT(ADDR(.text) - 0xC0000000)
  {
    /*code = .; _code = .; __code = .;*/
    *(.text)
  }

  /*. = ALIGN(4096);*/
  /*diff = . - 0xC0000000;*/

  /*.data ALIGN (4096) : AT(offset + diff)*/
  .data : AT(ADDR(.data) - 0XC0000000)
  {
     /*data = .; _data = .; __data = .;*/
     *(.data)
     *(.rodata)
     /*. = ALIGN(4096);*/
  }

  /*. = ALIGN(4096);*/
  /*diff = . - 0xC0000000;*/

  /*.bss ALIGN (4096) : AT(offset + diff)*/
  .bss : AT(ADDR(.bss) - 0xC0000000)
  {
    /*bss = .; _bss = .; __bss = .;*/
    *(.bss)
    /*. = ALIGN(4096);*/
  }

  end = .; _end = .; __end = .;
} 

