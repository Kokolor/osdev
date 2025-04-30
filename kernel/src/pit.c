//
// Created by kokolor on 30/04/25.
//

#include "io.h"
#include "pit.h"

uint64_t g_pit_ticks = 0;;

void pit_handler(const struct registers* registers)
{
    schedule((struct registers*)registers);
    g_pit_ticks++;
}

void pit_init(const uint32_t frequency)
{
    outb(0x43, 0x36);
    outb(0x40, 1193180 / frequency & 0xFF);
    outb(0x40, (1193180 / frequency >> 8) & 0xFF);
}