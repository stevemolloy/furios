#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include "isr.h"

void page_manager_init(u32int);
void remove_identityMB(u32int*);
void flushTLBPTE(u32int*);
void page_fault(registers_t);
void flushTLBPTErange(u32int*, u32int*);

#endif

