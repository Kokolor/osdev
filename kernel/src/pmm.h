//
// Created by kokolor on 29/04/25.
//

#ifndef PMM_H
#define PMM_H

uint64_t get_hhdm_offset(void);

// Alors ça, je mange du caca

#define BITMAP_SET(bitmap, idx) (bitmap[(idx) / 8] |= (1 << ((idx) % 8)))
#define BITMAP_CLEAR(bitmap, idx) (bitmap[(idx) / 8] &= ~(1 << ((idx) % 8)))
#define BITMAP_TEST(bitmap, idx) ((bitmap[(idx) / 8] >> ((idx) % 8)) & 1)

#define PHYS_TO_VIRT(addr) ((void*)((uint64_t)(addr) + get_hhdm_offset()))
#define VIRT_TO_PHYS(addr) ((void*)((uint64_t)(addr) - get_hhdm_offset()))

struct pmm_info
{
    struct limine_memmap_response* memmap;
    uint8_t* bitmap;
    uint64_t total_pages;
    uint64_t free_pages;
};

void* pmm_alloc_page(void);
void pmm_free_page(void* address);
void pmm_init(void);

#endif //PMM_H
