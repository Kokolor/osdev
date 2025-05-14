//
// Created by kokolor on 14/05/25.
//

#ifndef SYSCALL_H
#define SYSCALL_H

#include "idt.h"

void syscall_handler(const struct registers* registers);

#endif //SYSCALL_H
