//
// Created by kokolor on 29/04/25.
//

#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct gdt_ptr
{
    uint16_t limit;
    uint64_t address;
} __attribute__((packed));

void gdt_reload(struct gdt_ptr* gdt);
void segments_reload(void);
void gdt_init(void);

#endif //GDT_H
