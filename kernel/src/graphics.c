//
// Created by kokolor on 30/04/25.
//

#include <stddef.h>
#include "printf.h"
#include "font.h"
#include "limine.h"
#include "graphics.h"

uint32_t g_x, g_y = 0;
uint32_t g_color = 0xFFFFFF;

struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

const struct limine_framebuffer* g_framebuffer;

void set_pixel(const uint32_t x, const uint32_t y, const uint32_t color)
{
    uint32_t* framebuffer = g_framebuffer->address;
    framebuffer[y * (g_framebuffer->pitch / 4) + x] = color;
}

void set_rectangle(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height,
                   const uint32_t color)
{
    for (size_t i = x; i < x + width; i++)
    {
        for (size_t j = y; j < y + height; j++)
        {
            set_pixel(i, j, color);
        }
    }
}


void set_character(const char c, const int x, const int y, const uint32_t color)
{
    const uint32_t offset = 32 + (uint32_t)c * 16;

    for (int i = 0; i < 16; i++)
    {
        const uint8_t line = font[offset + i];

        for (int j = 0; j < 8; j++)
        {
            if (line & (0x80 >> j))
            {
                set_pixel(x + j, y + i, color);
            }
        }
    }
}

void set_string(const char* str, const int x, const int y, const uint32_t color)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        set_character(str[i], x + i * 8, y, color);
    }
}

void print_character(const char c)
{
    if (c == '\n' || g_x + 8 > g_framebuffer->width)
    {
        g_x = 0;
        g_y += 16;

        return;
    }

    set_character(c, g_x, g_y, g_color);
    g_x += 8;
}

void print_string(const char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        print_character(str[i]);
    }
}

// ReSharper disable once CppParameterMayBeConst
void _putchar(char character)
{
    print_character(character);
}

void framebuffer_init(void)
{
    g_framebuffer = framebuffer_request.response->framebuffers[0];
}
