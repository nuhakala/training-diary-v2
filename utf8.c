#include "utf8.h"
#include <sys/types.h>

#define isutf(c) (((c)&0xC0)!=0x80)
static const u_int32_t offsetsFromUTF8[6] = {
    0x00000000UL, 0x00003080UL, 0x000E2080UL,
    0x03C82080UL, 0xFA082080UL, 0x82082080UL
};

/* reads the next utf-8 sequence out of a string, updating an index */
u_int32_t u8_nextchar(char *s, int *i)
{
	u_int32_t ch = 0;
	int sz = 0;

	do {
		ch <<= 6;
		ch += (unsigned char)s[(*i)++];
		sz++;
	} while (s[*i] && !isutf(s[*i]));
	ch -= offsetsFromUTF8[sz - 1];

	return ch;
}

void u8_inc(char *s, int *i)
{
	(void)(isutf(s[++(*i)]) || isutf(s[++(*i)]) || isutf(s[++(*i)]) ||
	       ++(*i));
}

void u8_dec(char *s, int *i)
{
	(void)(isutf(s[--(*i)]) || isutf(s[--(*i)]) || isutf(s[--(*i)]) ||
	       --(*i));
}

/* number of characters */
int u8_strlen(char *s)
{
	int count = 0;
	int i = 0;

	while (u8_nextchar(s, &i) != 0)
		count++;

	return count;
}

int u8_offset(char *str, int charnum)
{
    int offs=0;

    while (charnum > 0 && str[offs]) {
        (void)(isutf(str[++offs]) || isutf(str[++offs]) ||
               isutf(str[++offs]) || ++offs);
        charnum--;
    }
    return offs;
}

int u8_charnum(char *s, int offset)
{
    int charnum = 0, offs=0;

    while (offs < offset && s[offs]) {
        (void)(isutf(s[++offs]) || isutf(s[++offs]) ||
               isutf(s[++offs]) || ++offs);
        charnum++;
    }
    return charnum;
}
