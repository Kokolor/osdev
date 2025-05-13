//
// Created by kokolor on 29/04/25.
//

#include "panic.h"
#include "idt.h"
#include "graphics.h"
#include "io.h"
#include "printf.h"

struct idt_entry g_idt_entries[256];
struct idt_ptr g_idt_ptr;
void* g_handlers[16];

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

void exception_handler(const struct registers* registers)
{
    if (registers->int_no < 32)
    {
        g_x = 0;
        g_y = 0;
        g_color = 0xFF0000;
        set_rectangle(0, 0, 240, 64, 0x000000);
        printf("RIP: 0x%x  RFLAGS: 0x%x\n", registers->rip, registers->rflags);
        printf("CS:  0x%x  SS: 0x%x\n", registers->cs, registers->ss);
        printf("Error Code: 0x%x\n", registers->error_code);

        panic(exception_messages[registers->int_no]);
    }
    else if (registers->int_no == 0x80)
    {
        printf("Syscall!");
    }
}

void idt_set_irq(const uint8_t irq, void* handler)
{
    g_handlers[irq] = handler;
}

void irq_handler(struct registers* registers)
{
    if (g_handlers[registers->int_no - 32])
    {
        ((void (*)(void*))g_handlers[registers->int_no - 32])(registers);
    }

    outb(0xA0, 0x20);
    outb(0x20, 0x20);
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

    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 4);
    outb(0xA1, 2);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    for (uint8_t vector = 0; vector < 48; vector++)
    {
        idt_set_entry(vector, isr_stub_table[vector], 0x8E);
    }

    idt_set_entry(0x80, isr_stub_table[128], 0xEE);

    idt_reload(&g_idt_ptr);
}
