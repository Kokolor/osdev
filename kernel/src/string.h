//
// Created by kokolor on 30/04/25.
//

#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* string);
int strncmp(const char* s1, const char* s2, size_t n);
void* memset(void* pointer, const int value, const size_t count);
void *memcpy(void *destination, const void *source, const size_t count);

#endif //STRING_H
