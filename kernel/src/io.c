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

uint8_t inb(uint16_t port)
{
    uint8_t data;
    asm volatile ("inb %1, %0" : "=a"(data) : "Nd"(port));

    return data;
}
