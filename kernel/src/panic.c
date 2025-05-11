//
// Created by kokolor on 29/04/25.
//

#include "printf.h"
#include "panic.h"

void panic(const char* message)
{
    printf("!! KERNEL PANIC !! %s", message);
    asm("cli; hlt");
}
