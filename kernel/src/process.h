//
// Created by kokolor on 30/04/25.
//

#ifndef PROCESS_H
#define PROCESS_H

#include "idt.h"

#define MAX_PROCESSES 10

typedef void (*process_entry)(void);

struct process
{
    int pid;
    int active;
    uint8_t* stack;
    uint8_t* stack_top;
    uint64_t* pml4;
    process_entry entry;
    struct registers registers;
};

void schedule(struct registers* registers);
int process_create(const process_entry entry);

#endif //PROCESS_H
