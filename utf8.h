#ifndef UTF_8_H_

/*
 * This header and corresponding implementations are copied from
 * https://www.cprogramming.com/tutorial/utf8.c
 * and are written by Jeff Bezanson. The corresponding code is under
 * public domain.
 *
 * Note, that I only copied parts that I needed.
 */

#include <sys/types.h>

u_int32_t u8_nextchar(char *s, int *i);
void u8_inc(char *s, int *i);
void u8_dec(char *s, int *i);
int u8_strlen(char *s);
int u8_offset(char *str, int charnum);
int u8_charnum(char *s, int offset);

#endif // !UTF_8_H_
