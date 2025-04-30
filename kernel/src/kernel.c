//
// Created by kokolor on 29/04/25.
//

#include "limine.h"
#include "gdt.h"
#include "graphics.h"
#include "heap.h"
#include "idt.h"
#include "io.h"
#include "pit.h"
#include "pmm.h"
#include "process.h"
#include "vmm.h"

__attribute__((used, section(".limine_requests")))
LIMINE_BASE_REVISION(3);

void task1(void)
{
    while (1)
    {
        set_rectangle(125, 125, 95, 45, 0x00FF00);
    }
}

void task2(void)
{
    while (1)
    {
        set_rectangle(15, 15, 75, 15, 0xFF0000);
    }
}

void entry()
{
    gdt_init();
    e9_printf("GDT Initialized");
    idt_init();
    e9_printf("IDT Initialized");
    pmm_init();
    e9_printf("PMM Initialized");
    vmm_init();
    e9_printf("VMM Initialized");

    framebuffer_init();

    void* heap_start = PHYS_TO_VIRT(pmm_alloc_page());
    heap_init(&g_kernel_heap, heap_start, 4 * 4096);

    pit_init(1000);
    process_create(task1);
    process_create(task2);
    idt_set_irq(0, pit_handler);

    while (1)
    {
        asm("hlt");
    }
}
