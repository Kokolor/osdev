//
// Created by kokolor on 29/04/25.
//

#include <stddef.h>

#include "limine.h"
#include "gdt.h"
#include "idt.h"
#include "io.h"
#include "pmm.h"

__attribute__((used, section(".limine_requests")))
LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests")))
struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void entry()
{
    const struct limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];
    gdt_init();
    idt_init();
    pmm_init();

    for (size_t i = 0; i < 100; i++)
    {
        volatile uint32_t* framebuffer_ptr = framebuffer->address;
        framebuffer_ptr[(framebuffer->pitch / 4) + i] = 0xffffff;
    }

    e9_printf("PMM Test Starting");

    e9_printf("\nAllocating 3 pages...");
    void* page1 = pmm_alloc_page();
    void* page2 = pmm_alloc_page();
    void* page3 = pmm_alloc_page();

    e9_printf("Page 1 allocated at: 0x%x", (uint64_t)page1);
    e9_printf("Page 2 allocated at: 0x%x", (uint64_t)page2);
    e9_printf("Page 3 allocated at: 0x%x", (uint64_t)page3);

    e9_printf("\nFreeing pages...");
    e9_printf("Freeing page 1 (0x%x)", (uint64_t)page1);
    pmm_free_page(page1);

    e9_printf("Freeing page 2 (0x%x)", (uint64_t)page2);
    pmm_free_page(page2);

    e9_printf("Freeing page 3 (0x%x)", (uint64_t)page3);
    pmm_free_page(page3);

    e9_printf("\nTesting reallocation...");
    void* new_page = pmm_alloc_page();
    e9_printf("Reallocated page at: 0x%x", (uint64_t)new_page);

    e9_printf("PMM Test Complete");


    asm("hlt; hlt");
}
