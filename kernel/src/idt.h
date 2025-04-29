//
// Created by kokolor on 29/04/25.
//

#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_entry
{
    uint16_t base_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t flags;
    uint16_t base_middle;
    uint32_t base_high;
    uint32_t reserved;
} __attribute__((packed));

struct idt_ptr
{
    uint16_t limit;
    uint64_t address;
} __attribute__((packed));

extern void* isr_stub_table[];

void idt_reload(struct idt_ptr* idt);
void idt_init(void);

#endif //IDT_H
