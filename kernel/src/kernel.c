//
// Created by kokolor on 29/04/25.
//

#include <stddef.h>

#include "limine.h"
#include "gdt.h"
#include "idt.h"

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

    for (size_t i = 0; i < 100; i++)
    {
        volatile uint32_t* framebuffer_ptr = framebuffer->address;
        framebuffer_ptr[(framebuffer->pitch / 4) + i] = 0xffffff;
    }

    asm("hlt; hlt");
}
