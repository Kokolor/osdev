//
// Created by kokolor on 29/04/25.
//

#include <stdio.h>

#include "printf.h"
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
        set_string("Task1", 125, 125, 0xFFFFFF);
        set_rectangle(120, 120, 450, 350, 0x00FF00);
    }
}

void task2(void)
{
    while (1)
    {
        set_string("Task2", 605, 405, 0xFFFFFF);
        set_rectangle(600, 400, 250, 200, 0xFF0000);
    }
}

void entry()
{
    framebuffer_init();

    pmm_init();
    printf("PMM Initialized\n");
    void* kstack = PHYS_TO_VIRT(pmm_alloc_page());
    gdt_init((uint64_t)kstack);
    printf("GDT Initialized\n");
    idt_init();
    printf("IDT Initialized\n");
    vmm_init();
    printf("VMM Initialized\n");

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
