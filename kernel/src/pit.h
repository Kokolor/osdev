//
// Created by kokolor on 30/04/25.
//

#ifndef PIT_H
#define PIT_H

#include "process.h"

void pit_handler(const struct registers* registers);
void pit_init(const uint32_t frequency);

#endif //PIT_H
