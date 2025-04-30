//
// Created by kokolor on 29/04/25.
//

#include "limine.h"
#include "gdt.h"
#include "heap.h"
#include "idt.h"
#include "io.h"
#include "pit.h"
#include "pmm.h"
#include "process.h"
#include "vmm.h"

__attribute__((used, section(".limine_requests")))
LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests")))
struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

const struct limine_framebuffer* g_framebuffer;

void task1(void)
{
    while (1)
    {
        e9_printf("1");
        for (volatile uint64_t i = 0; i < 1000000; i++)
        {
            __asm__ __volatile__("pause");
        }
    }
}

void task2(void)
{
    while (1)
    {
        e9_printf("2");
        for (volatile uint64_t i = 0; i < 1000000; i++)
        {
            __asm__ __volatile__("pause");
        }
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
    process_create(task1);
    process_create(task2);
    idt_set_irq(0, pit_handler);

    while (1)
    {
        asm("hlt");
    }
}
