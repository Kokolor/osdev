//
// Created by kokolor on 29/04/25.
//

#include <stdbool.h>
#include <stddef.h>

#include "limine.h"
#include "gdt.h"
#include "heap.h"
#include "idt.h"
#include "io.h"
#include "pmm.h"
#include "vmm.h"

__attribute__((used, section(".limine_requests")))
LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests")))
struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void pit_init(const uint32_t frequency)
{
    outb(0x43, 0x36);
    outb(0x40, 1193180 / frequency & 0xFF);
    outb(0x40, (1193180 / frequency >> 8) & 0xFF);
}

const struct limine_framebuffer* g_framebuffer;

uint64_t g_pit_ticks = 0;;

void pit_handler(const struct registers* registers)
{
    (void)registers;
    g_pit_ticks++;
    for (size_t i = 0; i < g_pit_ticks; i++)
    {
        volatile uint32_t* framebuffer_ptr = g_framebuffer->address;
        framebuffer_ptr[(g_framebuffer->pitch / 4) + i] = 0xff0000;
    }
}

void entry()
{
    g_framebuffer = framebuffer_request.response->framebuffers[0];
    gdt_init();
    e9_printf("GDT Initialized");
    idt_init();
    e9_printf("IDT Initialized");
    pmm_init();
    e9_printf("PMM Initialized");
    vmm_init();
    e9_printf("VMM Initialized");

    void* heap_start = PHYS_TO_VIRT(pmm_alloc_page());
    heap_init(&g_kernel_heap, heap_start, 4 * 4096);

    pit_init(1000);
    idt_set_irq(0, pit_handler);

    while (1)
    {
        asm("hlt");
    }
}
