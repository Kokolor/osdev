//
// Created by kokolor on 14/05/25.
//

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "idt.h"

void keyboard_handler(const struct registers* registers);
char keyboard_read(void);

#endif //KEYBOARD_H
