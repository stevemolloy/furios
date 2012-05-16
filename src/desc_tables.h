// Defining the descriptor tables (GDT, IDT)

#include "common.h"

struct gdt_segdesc_struct
{
    u16int limit_low;
    u16int base_low;
    u8int base_mid;
    u8int type;
    u8int other_info;
    u8int base_high;
} __attribute__((packed));  //Stop compiler "optimisations"
typedef struct gdt_segdesc_struct gdt_segdesc_t;

struct gdt_ptr_struct
{
    u16int limit;
    u32int base;
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

struct idt_ptr_struct
{
    u16int limit;
    u32int base;
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

struct idt_segdesc_struct
{
    u16int proc_offset_lo;
    u16int seg_sel;
    u8int  zeros;
    u8int  flag;
    u16int proc_offset_hi;
} __attribute__((packed));  //Stop compiler "optimisations"
typedef struct idt_segdesc_struct idt_segdesc_t;

void desc_tables_init();

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

