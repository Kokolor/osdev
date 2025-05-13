//
// Created by kokolor on 30/04/25.
//

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

extern uint32_t g_color;
extern uint32_t g_x, g_y;

void set_pixel(const uint32_t x, const uint32_t y, const uint32_t color);
void set_rectangle(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height,
                   const uint32_t color);
void set_string(const char* str, const int x, const int y, const uint32_t color);
void print_string(const char* str);
void framebuffer_init(void);

#endif //GRAPHICS_H
