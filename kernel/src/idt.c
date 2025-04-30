//
// Created by kokolor on 29/04/25.
//

#include "panic.h"
#include "idt.h"

#include "io.h"

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
    e9_printf("Registers dump:");
    e9_printf("RAX: %x  RBX: %x", registers->rax, registers->rbx);
    e9_printf("RCX: %x  RDX: %x", registers->rcx, registers->rdx);
    e9_printf("RSI: %x  RDI: %x", registers->rsi, registers->rdi);
    e9_printf("RBP: %x  RSP: %x", registers->rbp, registers->rsp);
    e9_printf("R8:  %x  R9:  %x", registers->r8, registers->r9);
    e9_printf("R10: %x  R11: %x", registers->r10, registers->r11);
    e9_printf("R12: %x  R13: %x", registers->r12, registers->r13);
    e9_printf("R14: %x  R15: %x", registers->r14, registers->r15);
    e9_printf("RIP: %x  RFLAGS: %x", registers->rip, registers->rflags);
    e9_printf("CS:  %x  SS: %x", registers->cs, registers->ss);
    e9_printf("Error Code: %x", registers->error_code);

    panic(exception_messages[registers->int_no]);
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

    idt_reload(&g_idt_ptr);
}
