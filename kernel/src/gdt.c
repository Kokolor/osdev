//
// Created by kokolor on 29/04/25.
//

#include "gdt.h"

struct tss_entry g_tss = {0};
struct gdt_ptr g_gdt_ptr;

uint64_t g_gdt_entries[7] = {0};

void gdt_set_tss(const int idx)
{
    const uint64_t base = (uint64_t)&g_tss;
    const uint64_t limit = sizeof(g_tss) - 1;

    g_gdt_entries[idx] = (limit & 0xFFFF) | ((base & 0xFFFFFF) << 16) | ((uint64_t)0x9 << 40) | ((uint64_t)0 << 44) | ((
        uint64_t)3 << 45) | ((uint64_t)1 << 47) | ((limit >> 16 & 0xF) << 48) | ((base >> 24 & 0xFF) << 56);

    g_gdt_entries[idx + 1] = (base >> 32) & 0xFFFFFFFF;
}

void gdt_init(void)
{
    uint64_t stack = 0;
    __asm__ volatile ("movq %%rsp, %0" : "=r"(stack));
    g_tss.rsp0 = stack + 0x4000; // 16 384 OMGGGG SKIBIDIIII QUOICOUBAKA LES COPAINS
    g_tss.io_map_base = sizeof(g_tss);

    g_gdt_entries[0] = 0x0000000000000000;
    g_gdt_entries[1] = 0x00af9b000000ffff;
    g_gdt_entries[2] = 0x00af93000000ffff;
    g_gdt_entries[3] = 0x00affb000000ffff;
    g_gdt_entries[4] = 0x00aff3000000ffff;
    g_gdt_entries[5] = 0;
    g_gdt_entries[6] = 0;
    gdt_set_tss(5);
    g_gdt_ptr.limit = sizeof(g_gdt_entries) - 1;
    g_gdt_ptr.address = (uint64_t)&g_gdt_entries;
    gdt_reload(&g_gdt_ptr);
    segments_reload();
    __asm__ volatile ("ltr %%ax" :: "a"((uint16_t)(5 * 8)));
}
