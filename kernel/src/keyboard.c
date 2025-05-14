//
// Created by kokolor on 14/05/25.
//

#include <stdint.h>
#include "keyboard.h"
#include "io.h"

char g_keyboard_buffer[245];
uint8_t g_head = 0;
uint8_t g_tail = 0;

char g_qwerty[256] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b', 0, 'q', 'w', 'e', 'r', 't', 'y', 'u',
    'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f',
    'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z',
    'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

void keyboard_handler(const struct registers* registers)
{
    (void)registers;
    const uint8_t scancode = inb(0x60);

    if (scancode < 0x80)
    {
        const char character = g_qwerty[scancode];

        if (character)
        {
            const uint8_t next = (g_head + 1) & 0xff;

            if (next != g_tail)
            {
                g_keyboard_buffer[g_tail] = character;
                g_head = next;
            }
        }
    }
}

char keyboard_read(void)
{
    while (g_head == g_tail)
    {
        asm volatile("sti; hlt; cli");
    }

    char character = g_keyboard_buffer[g_tail];
    g_tail = (g_tail + 1) & 0xff;

    return character;
}
