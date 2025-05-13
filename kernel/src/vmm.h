//
// Created by kokolor on 30/04/25.
//

#ifndef VMM_H
#define VMM_H

#include <stdint.h>

extern char text_start[];
extern char text_end[];
extern char rodata_start[];
extern char rodata_end[];
extern char data_start[];
extern char data_end[];

extern uint64_t* g_kernel_pml4;

uint64_t* vmm_new_pml4(void);
void vmm_switch_pml4(const uint64_t* pml4);
void vmm_map(const uint64_t* pml4, const uint64_t phys_addr, const uint64_t virt_addr, const uint64_t flags);
void vmm_init(void);

#endif //VMM_H
