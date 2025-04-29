//
// Created by kokolor on 30/04/25.
//

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