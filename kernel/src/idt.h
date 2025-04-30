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

struct registers
{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rbp;
    uint64_t rbx;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rax;
    uint64_t int_no;
    uint64_t error_code;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

extern void* isr_stub_table[];

void idt_reload(struct idt_ptr* idt);
void idt_set_irq(uint8_t irq, void* handler);
void idt_init(void);

#endif //IDT_H
