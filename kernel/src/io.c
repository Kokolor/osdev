//
// Created by kokolor on 29/04/25.
//

#include <stddef.h>
#include "io.h"

void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %%al, %%dx" :: "d" (port), "a" (value));
}

void e9_print_character(const char character)
{
    outb(0xE9, character);
}

void e9_print_string(const char* string)
{
    for (size_t i = 0; string[i] != '\0'; i++)
    {
        e9_print_character(string[i]);
    }
}
