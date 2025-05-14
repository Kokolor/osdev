//
// Created by kokolor on 30/04/25.
//

#include <stddef.h>
#include "string.h"
#include "pmm.h"
#include "vmm.h"
#include "process.h"

#include "printf.h"

struct process g_processes[MAX_PROCESSES];
int g_current_idx = -1;
uint8_t g_next_pid = 1;

void schedule(struct registers* registers)
{
    if (g_current_idx >= 0)
    {
        memcpy(&g_processes[g_current_idx].registers, registers, sizeof(*registers));
    }

    int next_process = g_current_idx;
    for (size_t i = 0; i < MAX_PROCESSES; i++)
    {
        next_process = (next_process + 1) % MAX_PROCESSES;
        if (g_processes[next_process].active)
        {
            break;
        }
    }

    if (!g_processes[next_process].active)
    {
        return;
    }

    vmm_switch_pml4(g_processes[next_process].pml4);
    memcpy(registers, &g_processes[next_process].registers, sizeof(struct registers));
    g_current_idx = next_process;
}

int process_create(const process_entry entry, uint64_t* pml4, const int ring)
{
    for (size_t i = 0; i < MAX_PROCESSES; i++)
    {
        if (!g_processes[i].active)
        {
            g_processes[i].pid = g_next_pid++;
            g_processes[i].active = 1;
            g_processes[i].entry = entry;
            g_processes[i].pml4 = pml4;
            uint8_t* buffer = PHYS_TO_VIRT(pmm_alloc_page());
            if (ring == RING_3)
            {
                vmm_map(g_processes[i].pml4, (uint64_t)VIRT_TO_PHYS(buffer), ((uint64_t)buffer), 1 | 2 | 4);
            }
            g_processes[i].stack = buffer;
            g_processes[i].stack_top = buffer + 4096;

            memset(&g_processes[i].registers, 0, sizeof(struct registers));

            g_processes[i].registers.rip = (uint64_t)entry;
            g_processes[i].registers.cs = (ring ? 0x1b : 0x08);
            g_processes[i].registers.ss = (ring ? 0x23 : 0x10);
            g_processes[i].registers.rflags = 0x202;
            g_processes[i].registers.rsp = (uint64_t)g_processes[i].stack_top;

            return g_processes[i].pid;
        }
    }

    return -1;
}
