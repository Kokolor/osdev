//
// Created by kokolor on 29/04/25.
//

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

uint64_t read_cr3(void)
{
    uint64_t value;
    __asm__ volatile ("mov %%cr3, %0" : "=r"(value) : : "memory");
    return value;
}

void entry()
{
    const struct limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];
    gdt_init();
    idt_init();
    pmm_init();
    vmm_init();

    for (size_t i = 0; i < 100; i++)
    {
        volatile uint32_t* framebuffer_ptr = framebuffer->address;
        framebuffer_ptr[(framebuffer->pitch / 4) + i] = 0xffffff;
    }

    e9_printf("Current CR3: %x", read_cr3());
    uint64_t* new_pml4 = vmm_new_pml4();
    vmm_switch_pml4(new_pml4);
    e9_printf("New CR3: %x", read_cr3());

    void* heap_start = PHYS_TO_VIRT(pmm_alloc_page());
    heap_init(&g_kernel_heap, heap_start, 4 * 4096);

    asm("hlt; hlt");
}
