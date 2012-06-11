#include "common.h"
#include "pagemanager.h"
#include "isr.h"
#include "monitor.h"

void page_manager_init(u32int pagedirloc)
{
    u32int *pagedirptr = (u32int*)pagedirloc;
    /*pagedirptr[0] = (u32int)0;*/
    remove_identityMB(pagedirptr);
    register_interrupt_handler(14, page_fault);
}

void remove_identityMB(u32int *pdptr)
{
    u32int addr = 0x0;
    pdptr[0] = (u32int)0;
    while (addr < 0x400000)
    {
        flushTLBPTE((u32int*)addr);
        addr += 0x1000;
    }
}

void flushTLBPTE(u32int *pte_addr)
{
    asm volatile ("invlpg %0" : : "m"(*pte_addr));
}

void page_fault(registers_t regs)
{
   /* A page fault has occurred.
   The faulting address is stored in the CR2 register.*/
   u32int faulting_address;

   /* The error code gives us details of what happened.*/
   int present   = !(regs.err_code & 0x1); /* Page not present*/
   int rw = regs.err_code & 0x2;           /* Write operation?*/
   int us = regs.err_code & 0x4;           /* Processor was in user-mode?*/
   int reserved = regs.err_code & 0x8;     /* Overwritten CPU-reserved bits of page entry?*/
   int id = regs.err_code & 0x10;          /* Caused by an instruction fetch?*/

   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   /* Output an error message.*/
   monitor_write("Page fault! ( ");
   if (present) {monitor_write("present ");}
   if (rw) {monitor_write("read-only ");}
   if (us) {monitor_write("user-mode ");}
   if (reserved) {monitor_write("reserved ");}
   if (id) {monitor_write("instruction fetch ");}
   monitor_write(") at 0x");
   monitor_write_hex(faulting_address);
   monitor_write("\n");
   PANIC("Page fault");
} 

