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

// Code from limine

const char CONVERSION_TABLE[] = "0123456789abcdef";

void e9_print_hexadecimal(size_t num)
{
    int i;
    char buf[17];

    if (!num)
    {
        e9_print_string("0x0");
        return;
    }

    buf[16] = 0;

    for (i = 15; num; i--)
    {
        buf[i] = CONVERSION_TABLE[num % 16];
        num /= 16;
    }

    i++;
    e9_print_string("0x");
    e9_print_string(&buf[i]);
}

void e9_print_decimal(size_t num)
{
    int i;
    char buf[21] = {0};

    if (!num)
    {
        e9_print_character('0');
        return;
    }

    for (i = 19; num; i--)
    {
        buf[i] = (num % 10) + 0x30;
        num = num / 10;
    }

    i++;
    e9_print_string(buf + i);
}

void e9_printf(const char* format, ...)
{
    va_list argp;
    va_start(argp, format);

    while (*format != '\0')
    {
        if (*format == '%')
        {
            format++;
            if (*format == 'x')
            {
                e9_print_hexadecimal(va_arg(argp, size_t));
            }
            else if (*format == 'd')
            {
                e9_print_decimal(va_arg(argp, size_t));
            }
            else if (*format == 's')
            {
                e9_print_string(va_arg(argp, char*));
            }
        }
        else
        {
            e9_print_character(*format);
        }
        format++;
    }

    e9_print_character('\n');
    va_end(argp);
}
