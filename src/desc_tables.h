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

void desc_tables_init();

