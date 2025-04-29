//
// Created by kokolor on 29/04/25.
//

#include "gdt.h"

struct gdt_ptr g_gdt_ptr;

uint64_t g_gdt_entries[5] = {
    0x0000000000000000,
    0x00af9b000000ffff,
    0x00af93000000ffff,
    0x00affb000000ffff,
    0x00aff3000000ffff,
};

void gdt_init(void)
{
    g_gdt_ptr.limit = sizeof(g_gdt_entries) - 1;
    g_gdt_ptr.address = (uint64_t)&g_gdt_entries;
    gdt_reload(&g_gdt_ptr);
    segments_reload();
}
