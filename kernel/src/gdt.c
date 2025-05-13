//
// Created by kokolor on 29/04/25.
//

#include "gdt.h"

struct tss_entry g_tss = {0};
struct gdt_ptr g_gdt_ptr;

uint64_t g_gdt_entries[7] = {0};

void gdt_init(uint64_t stack)
{
    g_tss.rsp0 = stack;
    g_tss.io_map_base = sizeof(g_tss);

    g_gdt_entries[0] = 0x0000000000000000;
    g_gdt_entries[1] = 0x00af9b000000ffff;
    g_gdt_entries[2] = 0x00af93000000ffff;
    g_gdt_entries[3] = 0x00affb000000ffff;
    g_gdt_entries[4] = 0x00aff3000000ffff;

    uint64_t tss_limit = sizeof(g_tss) - 1;
    uint64_t tss_address = (uint64_t)&g_tss;

    g_gdt_entries[5] = (tss_limit & 0xFFFF) | ((tss_address & 0xFFFFFF) << 16) | (0x9ULL << 40) | (1ULL << 47) | (3ULL
        << 45) | ((tss_limit >> 16 & 0xF) << 48) | ((tss_address >> 24 & 0xFF) << 56);

    g_gdt_entries[6] = (tss_address >> 32) & 0xFFFFFFFF;

    g_gdt_ptr.limit = sizeof(g_gdt_entries) - 1;
    g_gdt_ptr.address = (uint64_t)&g_gdt_entries;
    gdt_reload(&g_gdt_ptr);
    segments_reload();
    __asm__ volatile("ltr %%ax" :: "a"((uint16_t)(5 * 8)));
}
