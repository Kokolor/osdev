//
// Created by kokolor on 30/04/25.
//

#ifndef GRAPHICS_H
#define GRAPHICS_H

void set_pixel(const uint32_t x, const uint32_t y, const uint32_t color);
void set_rectangle(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height,
                   const uint32_t color);
void framebuffer_init(void);

#endif //GRAPHICS_H
