//
// Created by kokolor on 29/04/25.
//

#include "io.h"
#include "idt.h"

struct idt_entry g_idt_entries[256];
struct idt_ptr g_idt_ptr;

const char* exception_messages[32] = {
    "Division by zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Detected overflow",
    "Out-of-bounds",
    "Invalid opcode",
    "No coprocessor",
    "Double fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown interrupt",
    "Coprocessor fault",
    "Alignment check",
    "Machine check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void exception_handler(int int_number)
{
    e9_print_string("Exception: ");
    e9_print_string(exception_messages[int_number]);

    asm("cli; hlt");
}

void idt_set_entry(const uint8_t vector, void* isr, const uint8_t flags)
{
    struct idt_entry* descriptor = &g_idt_entries[vector];

    descriptor->base_low = (uint64_t)isr & 0xFFFF;
    descriptor->selector = 0x08;
    descriptor->ist = 0;
    descriptor->flags = flags;
    descriptor->base_middle = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->base_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
}

void idt_init(void)
{
    g_idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1;
    g_idt_ptr.address = (uint64_t)&g_idt_entries[0];

    for (uint8_t vector = 0; vector < 32; vector++)
    {
        idt_set_entry(vector, isr_stub_table[vector], 0x8E);
    }

    idt_reload(&g_idt_ptr);
}
