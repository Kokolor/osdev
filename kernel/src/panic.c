//
// Created by kokolor on 29/04/25.
//

#include "io.h"
#include "panic.h"

void panic(const char* message)
{
    e9_printf("!! KERNEL PANIC !! %s", message);
    asm("cli; hlt");
}
