//
// Created by kokolor on 29/04/25.
//

#include <stddef.h>
#include "limine.h"
#include "panic.h"
#include "io.h"
#include "pmm.h"

#include "printf.h"

struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

uint64_t get_hhdm_offset(void)
{
    return hhdm_request.response->offset;
}

struct pmm_info g_pmm_info;

void* find_bitmap_location(void)
{
    struct limine_memmap_entry** entries = g_pmm_info.memmap->entries;

    for (size_t i = 0; i < g_pmm_info.memmap->entry_count; i++)
    {
        if (entries[i]->type == LIMINE_MEMMAP_USABLE)
        {
            const uint64_t size_needed = (g_pmm_info.total_pages + 7) / 8;
            if (entries[i]->length >= size_needed)
            {
                return PHYS_TO_VIRT(entries[i]->base);
            }
        }
    }

    return NULL;
}

const char* get_memmap_type(const uint64_t type)
{
    switch (type)
    {
    case LIMINE_MEMMAP_USABLE:
        return "USABLE";
    case LIMINE_MEMMAP_RESERVED:
        return "RESERVED";
    case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
        return "ACPI_RECLAIMABLE";
    case LIMINE_MEMMAP_ACPI_NVS:
        return "ACPI_NVS";
    case LIMINE_MEMMAP_BAD_MEMORY:
        return "BAD_MEMORY";
    case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
        return "BOOTLOADER_RECLAIMABLE";
    case LIMINE_MEMMAP_KERNEL_AND_MODULES:
        return "KERNEL_AND_MODULES";
    case LIMINE_MEMMAP_FRAMEBUFFER:
        return "FRAMEBUFFER";
    default:
        return "UNKNOWN";
    }
}

void* pmm_alloc_page(void)
{
    for (uint64_t i = 0; i < g_pmm_info.total_pages; i++)
    {
        if (!BITMAP_TEST(g_pmm_info.bitmap, i))
        {
            BITMAP_SET(g_pmm_info.bitmap, i);
            g_pmm_info.free_pages--;

            return (void*)(i * 4096);
        }
    }

    return NULL;
}

void pmm_free_page(void* address)
{
    const uint64_t phys_address = (uint64_t)VIRT_TO_PHYS(address);
    const uint64_t page_index = phys_address / 4096;

    if (page_index >= g_pmm_info.total_pages)
    {
        return;
    }

    if (BITMAP_SET(g_pmm_info.bitmap, page_index))
    {
        BITMAP_CLEAR(g_pmm_info.bitmap, page_index);
        g_pmm_info.free_pages++;
    }
}

void pmm_init(void)
{
    g_pmm_info.memmap = memmap_request.response;

    struct limine_memmap_entry** entries = g_pmm_info.memmap->entries;

    g_pmm_info.total_pages = 0;
    for (size_t i = 0; i < g_pmm_info.memmap->entry_count; i++)
    {
        g_pmm_info.total_pages += (entries[i]->length + 4096 - 1) / 4096;
    }

    g_pmm_info.bitmap = find_bitmap_location();
    if (g_pmm_info.bitmap == NULL)
    {
        panic("Could not found bitmap location");
    }

    for (size_t i = 0; i < (g_pmm_info.total_pages + 7) / 8; i++)
    {
        g_pmm_info.bitmap[i] = 0xFF;
    }

    g_pmm_info.free_pages = 0;
    for (size_t i = 0; i < g_pmm_info.memmap->entry_count; i++)
    {
        if (entries[i]->type == LIMINE_MEMMAP_USABLE)
        {
            const uint64_t start_page = entries[i]->base / 4096;
            const uint64_t page_count = entries[i]->length / 4096;

            for (uint64_t j = 0; j < page_count; j++)
            {
                BITMAP_CLEAR(g_pmm_info.bitmap, start_page + j);
                g_pmm_info.free_pages++;
            }
        }
    }

    const uint64_t bitmap_pages = ((g_pmm_info.total_pages + 7) / 8 + 4096 - 1) / 4096;
    const uint64_t bitmap_start = (uint64_t)VIRT_TO_PHYS(g_pmm_info.bitmap) / 4096;

    for (uint64_t i = 0; i < bitmap_pages; i++)
    {
        BITMAP_SET(g_pmm_info.bitmap, bitmap_start + i);
        g_pmm_info.free_pages--;
    }
}
