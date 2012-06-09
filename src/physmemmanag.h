#ifndef PHYSMEMMANAG_H
#define PHYSMEMMANAG_H
#include "multiboot.h"

/*void physmemstack_init(u32int, u32int);*/
void physmemstack_init(multiboot_info_t*, u32int);
void fill_stack(multiboot_info_t*, u32int);
void push_memrange(u32int, u32int);
void push_pmstack(u32int);
u32int pop_pmstack();

#endif

