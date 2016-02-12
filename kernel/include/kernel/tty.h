#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

void terminal_init(void);
void terminal_putchar(char c);
void terminal_write(char* data, size_t size);
void terminal_writestring(char* data);

#endif
