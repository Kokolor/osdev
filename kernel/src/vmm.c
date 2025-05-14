//
// Created by kokolor on 30/04/25.
//

#include "string.h"
#include "limine.h"
#include "pmm.h"
#include "vmm.h"

#include "printf.h"

// Thanks again to https://github.com/asterd-og/ZanOS/

struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

uint64_t* g_kernel_pml4;

uint64_t vmm_get_pte_address(const uint64_t value)
{
    return value & 0x000ffffffffff000;
}

uint64_t* vmm_new_pml4(void)
{
    uint64_t* pml4 = PHYS_TO_VIRT(pmm_alloc_page());
    memset(pml4, 0, 4096);

    for (size_t i = 0; i < 512; i++)
    {
        pml4[i] = g_kernel_pml4[i];
    }

    return pml4;
}

void vmm_switch_pml4(const uint64_t* pml4)
{
    __asm__ volatile ("mov %0, %%cr3" : : "r"((uint64_t)VIRT_TO_PHYS(pml4)) : "memory");
}

uint64_t* vmm_get_next_level(uint64_t* level, const uint64_t entry, const uint64_t flags)
{
    if (level[entry] & 1)
        return PHYS_TO_VIRT(vmm_get_pte_address(level[entry]));

    uint64_t* pml = PHYS_TO_VIRT(pmm_alloc_page());
    memset(pml, 0, 4096);
    level[entry] = (uint64_t)VIRT_TO_PHYS(pml) | (flags | 0x4);

    return pml;
}

void vmm_map(const uint64_t* pml4, const uint64_t phys_addr, const uint64_t virt_addr, const uint64_t flags)
{
    const uint64_t pml1_entry = (virt_addr >> 12) & 0x1ff;
    const uint64_t pml2_entry = (virt_addr >> 21) & 0x1ff;
    const uint64_t pml3_entry = (virt_addr >> 30) & 0x1ff;
    const uint64_t pml4_entry = (virt_addr >> 39) & 0x1ff;

    uint64_t* pml3 = vmm_get_next_level((uint64_t*)pml4, pml4_entry, 1 | 2);
    uint64_t* pml2 = vmm_get_next_level(pml3, pml3_entry, 1 | 2);
    uint64_t* pml1 = vmm_get_next_level(pml2, pml2_entry, 1 | 2);

    pml1[pml1_entry] = phys_addr | flags;
}

void vmm_init(void)
{
    g_kernel_pml4 = (uint64_t*)PHYS_TO_VIRT(pmm_alloc_page());
    memset(g_kernel_pml4, 0, 4096);

    const uint64_t phys_base = kernel_address_request.response->physical_base;
    const uint64_t virt_base = kernel_address_request.response->virtual_base;

    for (uint64_t addr = (uint64_t)&text_start; addr < (uint64_t)&text_end; addr += 4096)
    {
        vmm_map(g_kernel_pml4, addr - virt_base + phys_base, addr, 1);
    }

    for (uint64_t addr = (uint64_t)&rodata_start; addr < (uint64_t)&rodata_end; addr += 4096)
    {
        vmm_map(g_kernel_pml4, addr - virt_base + phys_base, addr, 1 | (1ULL << 63));
    }

    for (uint64_t addr = (uint64_t)&data_start; addr < (uint64_t)&data_end; addr += 4096)
    {
        vmm_map(g_kernel_pml4, addr - virt_base + phys_base, addr, 1 | 2 | (1ULL << 63));
    }

    for (uint64_t addr = 0; addr < 0x100000000; addr += 4096)
    {
        vmm_map(g_kernel_pml4, addr, addr, 1 | 2);
        vmm_map(g_kernel_pml4, addr, (uint64_t)PHYS_TO_VIRT(addr), 1 | 2);
    }

    vmm_switch_pml4(g_kernel_pml4);
}
