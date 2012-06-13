/* Manage the physical memory */

#include "common.h"
#include "physmemmanag.h"
#include "multiboot.h"
#include "monitor.h"

extern const void end;
extern const u32int *physstack_start;
extern const u32int *physstack_end;
extern u32int *physstack_ptr;

void physmemstack_init(multiboot_info_t *mbi, u32int kernelend)
{
    u32int stacksize, totmem=mbi->mem_lower + mbi->mem_upper;

    kernelend++; /*Don't clobber the last byte of the kernel*/

    /* If not 4096 byte aligned, then align it*/
    if ((kernelend & 0xFFFFF000) != kernelend)
    {
        kernelend = (kernelend & 0xFFFFF000) + 0x00001000;
    }
    totmem *= 1024; /* Convert from kB to B*/

    /* T = U + S  (total_avail = usable+stack
    S = U/4096 * 4
    Therefore S = (T - K) / 1025*/
    stacksize = totmem / 1025;
    if (totmem % 1025 != 0)
        stacksize++; /* Round up */

    /* Bottom of this stack is stacksize bytes after the kernel ends*/
    physstack_start = (u32int*)kernelend;
    physstack_end   = (u32int*)(kernelend + stacksize);
    physstack_ptr   = (u32int*)kernelend;

    fill_stack(mbi, kernelend);
}

void fill_stack(multiboot_info_t *mbi, u32int kend)
{
    u32int length;
    /*push_memrange(0x400000, totmem-0x400000);*/
    memory_map_t *mmap = (memory_map_t*)mbi->mmap_addr;
    monitor_write("Memory map received from GRUB\n");
    monitor_write("        Base        Length      Type\n");
    /* Loop to print out the mem map, and fill the mem stack appropriately*/
    while (mmap < (memory_map_t*)(mbi->mmap_addr + mbi->mmap_length))
    {
        monitor_write("        0x");
        monitor_write_hex(mmap->base_addr_low);
        monitor_write("  ");
        monitor_write("0x");
        monitor_write_hex(mmap->length_low);
        monitor_write("  ");
        if (mmap->type==1)
            monitor_write("Avail\n");
        else
            monitor_write("Not avail\n");

        /*If the mem is available and over 1MB, then push it onto the stack
        The restriction on >1MB isn't quite necessary, but confirming the lower
        memory is free doesn't sound easy...*/
        if (mmap->type == 1 && mmap->base_addr_low >= 0x100000)
        {
            /*The kernel takes up space after 1MB, so only push pages after kend*/
            length = mmap->length_low - (kend - mmap->base_addr_low);
            push_memrange(kend, length);
        }

        mmap = (memory_map_t*) ((u32int)mmap + mmap->size + sizeof(u32int));
    }
}

void push_memrange(u32int lower, u32int length)
{
    u32int step=0;

    while (step < length)
    {
        push_pmstack(lower+step);
        step += 4096;
    }
}

/* A push function.  Pushes an address onto the stack*/
void push_pmstack(u32int paddr)
{
    if (physstack_ptr >= physstack_end)
        PANIC("Attempted to overfill phys mem stack!");

    *(physstack_ptr++) = (u32int)paddr;
}

/* Pops an address from the stack and returns it*/
u32int pop_pmstack()
{
    if (--physstack_ptr < physstack_start)
    {
        physstack_ptr++;
        PANIC("Out of physical memory!");
    }
    return *(physstack_ptr);
}

