//
// Created by kokolor on 30/04/25.
//

#include <stdint.h>
#include "string.h"

size_t strlen(const char* string)
{
    size_t len = 0;
    while (string[len]) len++;

    return len;
}


int strncmp(const char* s1, const char* s2, size_t n)
{
    while (n && *s1 && (*s1 == *s2))
    {
        ++s1;
        ++s2;
        --n;
    }

    if (n == 0)
    {
        return 0;
    }

    return (*(unsigned char*)s1 - *(unsigned char*)s2);
}

void* memset(void* pointer, const int value, const size_t count)
{
    uint8_t* ptr = (uint8_t*)pointer;

    for (size_t i = 0; i < count; i++)
    {
        ptr[i] = (uint8_t)value;
    }

    return ptr;
}

void* memcpy(void* destination, const void* source, const size_t count)
{
    uint8_t* dest = (uint8_t*)destination;
    const uint8_t* src = (const uint8_t*)source;

    for (size_t i = 0; i < count; i++)
        dest[i] = src[i];

    return destination;
}
