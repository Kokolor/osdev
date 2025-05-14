//
// Created by kokolor on 13/05/25.
//

#include "elf.h"
#include "pmm.h"
#include "printf.h"
#include "process.h"
#include "string.h"
#include "vmm.h"

int elf64_is_valid(Elf64_Ehdr* hdr)
{
    if (*(uint32_t*)hdr != 0x464C457F)
    {
        printf("[ELF] Invalid magic\n");
        return 0;
    }

    if (hdr->e_ident[4] != 2)
    {
        printf("[ELF] Not 64-bit\n");
        return 0;
    }

    return 1;
}

void* elf_load_file(void* file, const uint64_t* pml4, const int ring)
{
    Elf64_Ehdr* header = file;

    if (!elf64_is_valid(header))
        return NULL;

    const Elf64_Phdr* program_header = (Elf64_Phdr*)((uint8_t*)file + header->e_phoff);

    for (int i = 0; i < header->e_phnum; i++)
    {
        if (program_header[i].p_type != 1)
            continue;

        const uint64_t virt = program_header[i].p_vaddr;
        const uint64_t offset = program_header[i].p_offset;
        const uint64_t filesz = program_header[i].p_filesz;
        const uint64_t memsz = program_header[i].p_memsz;

        for (uint64_t off = 0; off < memsz; off += 0x1000)
        {
            void* phys = pmm_alloc_page();
            if (ring == RING_0)
                vmm_map(pml4, (uint64_t)phys, virt + off, 1 | 2);
            else
                vmm_map(pml4, (uint64_t)phys, virt + off, 1 | 2 | 4);
        }

        memcpy((void*)virt, (uint8_t*)file + offset, filesz);
        memset((void*)(virt + filesz), 0, memsz - filesz);
    }

    return (void*)header->e_entry;
}
