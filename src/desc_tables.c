// Defining the descriptor tables (GDT, IDT)

#include "desc_tables.h"
#include "monitor.h"

static void add_seg_desc(s32int, u32int, u32int, u8int, u8int);
static void init_gdt();
extern void gdt_gen(u32int);

gdt_segdesc_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

void desc_tables_init()
{
    init_gdt();
}

void init_gdt()
{
    u32int base  = 0x0; // Memory starts at zero!
    u32int limit = 0xFFFFFFFF; // Address all 4 GiB (even if they don't exist!)

    add_seg_desc(0, 0, 0, 0, 0); // Null descriptor expected by many systems
    add_seg_desc(1, base, limit, 0x9C, 0xC0); // Code, ring=0
    add_seg_desc(2, base, limit, 0x92, 0xC0); // Data, ring=0
    add_seg_desc(3, base, limit, 0xFC, 0xC0); // Code, ring=3
    add_seg_desc(4, base, limit, 0xF2, 0xC0); // Data, ring=3

    gdt_ptr.base  = (u32int)&gdt_entries;
    gdt_ptr.limit = (sizeof(gdt_segdesc_t)*5 - 1);

    monitor_write("Triple fault, here we come!!!");
    gdt_gen((u32int)&gdt_ptr);
}

void add_seg_desc(s32int num, u32int base, u32int limit, u8int type, u8int other_info)
{
    gdt_entries[num].base_low   = (u16int)(base & 0xFFFF);
    gdt_entries[num].base_mid   = (u8int)((base >> 16) & 0xFF);
    gdt_entries[num].base_high  = (u8int)((base >> 24) & 0xFF);

    gdt_entries[num].limit_low  = (u16int)(limit & 0xFFFF);

    gdt_entries[num].type       = type;

    // The other_info u16int is composed of the lower 4 bits of the other_info input
    // plus bits 16:19 (inc, zero-indexed) of the limit
    gdt_entries[num].other_info = (u8int)(other_info & 0xF0) | (u8int)((limit >> 16) & 0x0F);
}

