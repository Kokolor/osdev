//
// Created by kokolor on 29/04/25.
//

#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct tss_entry
{
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist[7];
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t io_map_base;
} __attribute__((packed));

struct gdt_ptr
{
    uint16_t limit;
    uint64_t address;
} __attribute__((packed));

void gdt_reload(struct gdt_ptr* gdt);
void segments_reload(void);
void gdt_init(void);

#endif //GDT_H
