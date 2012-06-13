#include "common.h"
#include "pagemanager.h"
#include "isr.h"
#include "monitor.h"

extern const u32int *physstack_end;

void page_manager_init(u32int pagedirloc)
{
    u32int pte, *pteptr, blankingptr;
    /* Find the 1st 4kB aligned page after the end of our code+physmemstack*/
    u32int blank = (((u32int)physstack_end) & 0xFFFFF000) + 0x1000;
    u32int *pagedirptr = (u32int*)pagedirloc;

    /* Divide by 4096 to get an index to the PTE*/
    blank /= 0x1000;

    /* Register the page fault handler */
    register_interrupt_handler(14, &page_fault);

    /* Zero out the PDE pointing to the identity mapped 1st 1MB*/
    remove_identityMB(pagedirptr);

    monitor_write("Zeroing PTEs from 0x");
    monitor_write_hex(blank*0x1000);
    monitor_write("\n");
    /*Get a pointer to the appropriate PTE, remembering to divide by
    4 since we're dealing with a 32bit ptr*/
    pte = ((u32int*)(pagedirloc+0xC0000000))[0xC00/4];
    /*Zero the lower 12 bits in order to get a raw address*/
    pte &= 0xFFFFF000;
    /*Create a pointer to the virtual location of this phys addr*/
    pteptr = (u32int*)(pte+0xc0000000);
    /*pteptr is a pointer to the base of the relevant PTE, so
    pteptr[i] will be the value at the memory location offset i
    from that base.
    So we loop from blank (created a few lines ago) to the end of
    the PTE, setting each value to zero, and invlpg'ing it.*/

    while (blank < 0x400)
    {
        blankingptr = pteptr[blank] & 0xFFFFF000;
        pteptr[blank] = 0;
        flushTLBPTE((u32int*) blankingptr);
        blank++;
    }
}

void remove_identityMB(u32int *pdptr)
{
    pdptr[0] = (u32int)0;
    flushTLBPTErange((u32int*)0x0, (u32int*)0x400000);
}

void flushTLBPTErange(u32int *first, u32int *last)
{
    while (first < last)
    {
        flushTLBPTE(first);
        /*Divide by 4 since first is a 32bit ptr*/
        first += 0x1000/4;
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
   int present  = !(regs.err_code & 0x1); /* Page not present*/
   int rw       =   regs.err_code & 0x2;  /* Write operation?*/
   int us       =   regs.err_code & 0x4;  /* Processor was in user-mode?*/
   int reserved =   regs.err_code & 0x8;  /* Overwrote reserved bits of PTE?*/
   int id       =   regs.err_code & 0x10; /* Caused by an instruction fetch?*/

   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   /* Output an error message.*/
   monitor_write("Page fault! --> ");
   if (present) {monitor_write("present ");}
   if (rw) {monitor_write("read-only ");}
   if (us) {monitor_write("user-mode ");}
   if (reserved) {monitor_write("reserved ");}
   if (id) {monitor_write("instruction fetch ");}
   monitor_write("at 0x");
   monitor_write_hex(faulting_address);
   monitor_write("\n");
   PANIC("Page fault");
} 

