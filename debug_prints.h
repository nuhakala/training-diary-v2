#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

#include <stdio.h>

#ifdef DEBUG
#define d_printf(fmt, args...) printf("DEBUG: %s:%d: " fmt, __FILE__, __LINE__, ##args)
#else
#define d_printf(fmt, args...)
#endif // !DEBUG

#endif // !DEBUG_PRINT_H
