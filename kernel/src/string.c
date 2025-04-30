//
// Created by kokolor on 30/04/25.
//

#include <stdint.h>
#include "string.h"

void* memset(void* pointer, const int value, const size_t count)
{
    uint8_t* ptr = (uint8_t*)pointer;

    for (size_t i = 0; i < count; i++)
    {
        ptr[i] = (uint8_t)value;
    }

    return ptr;
}

void* memcpy(void* destination, const void* source, const size_t count) {
    uint8_t *dest = (uint8_t *)destination;
    const uint8_t *src = (const uint8_t *)source;

    for (size_t i = 0; i < count; i++)
        dest[i] = src[i];

    return destination;
}
