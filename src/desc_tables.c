/*Defining the descriptor tables (GDT, IDT)*/

#include "desc_tables.h"
#include "monitor.h"

static void add_seg_desc(s32int, u32int, u32int, u8int, u8int);
static void init_gdt();
static void add_idt_desc(s32int, s32int, u8int, u8int);
static void init_idt();
extern void gdt_gen(u32int);
extern void idt_gen(u32int);

gdt_segdesc_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

idt_segdesc_t idt_entries[256];
idt_ptr_t idt_ptr;

void desc_tables_init()
{
    init_gdt();
    init_idt();
}

void init_gdt()
{
    u32int base  = 0x0; 
    u32int limit = 0xFFFFFFFF; 

    add_seg_desc(0, 0, 0, 0, 0); 
    add_seg_desc(1, base, limit, 0x9A, 0xC0); 
    add_seg_desc(2, base, limit, 0x92, 0xC0); 
    add_seg_desc(3, base, limit, 0xFA, 0xC0); 
    add_seg_desc(4, base, limit, 0xF2, 0xC0); 

    gdt_ptr.base  = (u32int)&gdt_entries;
    gdt_ptr.limit = (sizeof(gdt_segdesc_t)*5 - 1);

    gdt_gen((u32int)&gdt_ptr);
}

void add_seg_desc(s32int num, u32int base, u32int limit, u8int type, u8int other_info)
{
    gdt_entries[num].base_low   = (u16int)(base & 0xFFFF);
    gdt_entries[num].base_mid   = (u8int)((base >> 16) & 0xFF);
    gdt_entries[num].base_high  = (u8int)((base >> 24) & 0xFF);

    gdt_entries[num].limit_low  = (u16int)(limit & 0xFFFF);

    gdt_entries[num].type       = type;

    gdt_entries[num].other_info = (u8int)(other_info & 0xF0) | (u8int)((limit >> 16) & 0x0F);
}

void init_idt()
{
    /*Remap the irq table.*/
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    add_idt_desc(0, (u32int)isr0, 0x08, 0x8E);
    add_idt_desc(1, (u32int)isr1, 0x08, 0x8E);
    add_idt_desc(2, (u32int)isr2, 0x08, 0x8E);
    add_idt_desc(3, (u32int)isr3, 0x08, 0x8E);
    add_idt_desc(4, (u32int)isr4, 0x08, 0x8E);
    add_idt_desc(5, (u32int)isr5, 0x08, 0x8E);
    add_idt_desc(6, (u32int)isr6, 0x08, 0x8E);
    add_idt_desc(7, (u32int)isr7, 0x08, 0x8E);
    add_idt_desc(8, (u32int)isr8, 0x08, 0x8E);
    add_idt_desc(9, (u32int)isr9, 0x08, 0x8E);
    add_idt_desc(10, (u32int)isr10, 0x08, 0x8E);
    add_idt_desc(11, (u32int)isr11, 0x08, 0x8E);
    add_idt_desc(12, (u32int)isr12, 0x08, 0x8E);
    add_idt_desc(13, (u32int)isr13, 0x08, 0x8E);
    add_idt_desc(14, (u32int)isr14, 0x08, 0x8E);
    add_idt_desc(15, (u32int)isr15, 0x08, 0x8E);
    add_idt_desc(16, (u32int)isr16, 0x08, 0x8E);
    add_idt_desc(17, (u32int)isr17, 0x08, 0x8E);
    add_idt_desc(18, (u32int)isr18, 0x08, 0x8E);
    add_idt_desc(19, (u32int)isr19, 0x08, 0x8E);
    add_idt_desc(20, (u32int)isr20, 0x08, 0x8E);
    add_idt_desc(21, (u32int)isr21, 0x08, 0x8E);
    add_idt_desc(22, (u32int)isr22, 0x08, 0x8E);
    add_idt_desc(23, (u32int)isr23, 0x08, 0x8E);
    add_idt_desc(24, (u32int)isr24, 0x08, 0x8E);
    add_idt_desc(25, (u32int)isr25, 0x08, 0x8E);
    add_idt_desc(26, (u32int)isr26, 0x08, 0x8E);
    add_idt_desc(27, (u32int)isr27, 0x08, 0x8E);
    add_idt_desc(28, (u32int)isr28, 0x08, 0x8E);
    add_idt_desc(29, (u32int)isr29, 0x08, 0x8E);
    add_idt_desc(30, (u32int)isr30, 0x08, 0x8E);
    add_idt_desc(31, (u32int)isr31, 0x08, 0x8E);
    /*The following are the IRQ interrupts*/
    add_idt_desc(32, (u32int)irq0, 0x08, 0x8E);
    add_idt_desc(33, (u32int)irq1, 0x08, 0x8E);
    add_idt_desc(34, (u32int)irq2, 0x08, 0x8E);
    add_idt_desc(35, (u32int)irq3, 0x08, 0x8E);
    add_idt_desc(36, (u32int)irq4, 0x08, 0x8E);
    add_idt_desc(37, (u32int)irq5, 0x08, 0x8E);
    add_idt_desc(38, (u32int)irq6, 0x08, 0x8E);
    add_idt_desc(39, (u32int)irq7, 0x08, 0x8E);
    add_idt_desc(40, (u32int)irq8, 0x08, 0x8E);
    add_idt_desc(41, (u32int)irq9, 0x08, 0x8E);
    add_idt_desc(42, (u32int)irq10, 0x08, 0x8E);
    add_idt_desc(43, (u32int)irq11, 0x08, 0x8E);
    add_idt_desc(44, (u32int)irq12, 0x08, 0x8E);
    add_idt_desc(45, (u32int)irq13, 0x08, 0x8E);
    add_idt_desc(46, (u32int)irq14, 0x08, 0x8E);
    add_idt_desc(47, (u32int)irq15, 0x08, 0x8E);

    idt_ptr.base  = (u32int)&idt_entries;
    idt_ptr.limit = (sizeof(idt_segdesc_t)*256 - 1);

    idt_gen((u32int)&idt_ptr);
}

void add_idt_desc(s32int num, s32int handleraddr, u8int segsel, u8int flag)
{
    idt_entries[num].proc_offset_lo = handleraddr & 0xFFFF;
    idt_entries[num].proc_offset_hi = handleraddr >> 16 & 0xFFFF;
    idt_entries[num].seg_sel = segsel;
    idt_entries[num].flag = flag;
    idt_entries[num].zeros = 0x00;
}

