//
// Created by kokolor on 29/04/25.
//

#include <stdarg.h>
#include <stddef.h>
#include "io.h"

void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %%al, %%dx" :: "d" (port), "a" (value));
}
