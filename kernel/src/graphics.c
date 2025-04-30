//
// Created by kokolor on 30/04/25.
//

#include <stddef.h>
#include "limine.h"
#include "graphics.h"

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

void framebuffer_init(void)
{
    g_framebuffer = framebuffer_request.response->framebuffers[0];
}
