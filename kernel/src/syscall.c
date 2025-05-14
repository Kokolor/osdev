//
// Created by kokolor on 14/05/25.
//

#include "syscall.h"
#include "graphics.h"
#include "keyboard.h"
#include "printf.h"

void syscall_handler(const struct registers* registers)
{
    switch (registers->rax)
    {
    case 1: // void write(const char* string);
        print_string((const char*)registers->rdi);
        break;
    case 2: // void set_pixel(const uint32_t x, const uint32_t y, const uint32_t color):
        set_pixel((uint32_t)registers->rdi, (uint32_t)registers->rsi, (uint32_t)registers->rdx);
        break;
    case 3:
        // void set_rectangle(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const uint32_t color);
        set_rectangle((uint32_t)registers->rdi, (uint32_t)registers->rsi, (uint32_t)registers->rdx,
                      (uint32_t)registers->r10, (uint32_t)registers->r8);
        break;
    case 4: // char getchar(void);
        ((struct registers*)registers)->rax = (uint64_t)keyboard_read();
        break;
    default:
        printf("Unknown syscall: %d\n", registers->rax);
        break;
    }
}
