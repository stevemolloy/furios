#include "common.h"
#include "pagemanager.h"

void page_manager_init(u32int pagedirloc)
{
    u32int *pagedirptr = (u32int*)pagedirloc;

    remove_identityMB(pagedirptr);
}

void remove_identityMB(u32int *pdptr)
{
    pdptr[0] = (u32int)0;
}

