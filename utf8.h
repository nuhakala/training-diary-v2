#ifndef UTF_8_H_
#define UTF_8_H_

/*
 * Most of this header and corresponding implementations are copied from
 * https://www.cprogramming.com/tutorial/unicode.html
 * and are written by Jeff Bezanson. The corresponding code is under
 * public domain.
 *
 * Note, that I only copied parts that I needed.
 */

#include <sys/types.h>
#include "windows_types.h"

#define isutf(c) (((c)&0xC0)!=0x80)

u_int32_t u8_nextchar(char *s, int *i);
void u8_inc(char *s, int *i);
void u8_dec(char *s, int *i);
int u8_strlen(char *s);
int u8_offset(char *str, int charnum);
int u8_charnum(char *s, int offset);
int u8_toucs(u_int32_t *dest, int sz, char *src, int srcsz);

// This function is written by me
int compare_strings_u8(char * a, char * b);

#endif // !UTF_8_H_
