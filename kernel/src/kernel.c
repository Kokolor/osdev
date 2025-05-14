//
// Created by kokolor on 29/04/25.
//

#include <stdio.h>

#include "elf.h"
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
#include "string.h"
#include "tar.h"
#include "vmm.h"

__attribute__((used, section(".limine_requests")))
LIMINE_BASE_REVISION(3);

struct limine_internal_module ramdisk = {
    .path = "/ramdisk.tar"
};

struct limine_internal_module* modules[] = {
    &ramdisk
};

struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 1,
    .response = NULL,
    .internal_module_count = 1,
    .internal_modules = modules
};

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

    struct limine_file* file = module_request.response->modules[0];
    printf("Found ramdisk: %s\n", file->path);
    tar_init(file->address);

    void* heap_start = PHYS_TO_VIRT(pmm_alloc_page());
    heap_init(&g_kernel_heap, heap_start, 4 * 4096);

    struct tar_handle loop_handle;
    uint64_t* pml4 = vmm_new_pml4();

    g_x = 0;
    g_y = 0;

    if (tar_open("./hello.elf", &loop_handle) == 0)
    {
        void* entry = elf_load_file(loop_handle.data, pml4, RING_3);
        if (entry)
        {
            process_create(entry, pml4, RING_3);
        }
        else
        {
            printf("Error load ELF (skibidi neuil)\n");
        }
    }

    pit_init(1000);
    idt_set_irq(0, pit_handler);

    while (1)
    {
        asm("hlt");
    }
}
