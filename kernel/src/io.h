//
// Created by kokolor on 29/04/25.
//

#ifndef IO_H
#define IO_H

#include <stdint.h>

void outb(uint16_t port, uint8_t value);
void e9_printf(const char* format, ...);

#endif //IO_H
