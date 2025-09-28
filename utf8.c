#include <sys/types.h>

#include "utf8.h"

static const u_int32_t offsetsFromUTF8[6] = {
    0x00000000UL, 0x00003080UL, 0x000E2080UL,
    0x03C82080UL, 0xFA082080UL, 0x82082080UL
};

static const char trailingBytesForUTF8[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
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

int u8_toucs(u_int32_t *dest, int sz, char *src, int srcsz)
{
    u_int32_t ch;
    char *src_end = src + srcsz;
    int nb;
    int i=0;

    while (i < sz-1) {
        nb = trailingBytesForUTF8[(unsigned char)*src];
        if (srcsz == -1) {
            if (*src == 0)
                goto done_toucs;
        }
        else {
            if (src + nb >= src_end)
                goto done_toucs;
        }
        ch = 0;
        switch (nb) {
            /* these fall through deliberately */
        case 3: ch += (unsigned char)*src++; ch <<= 6;
        case 2: ch += (unsigned char)*src++; ch <<= 6;
        case 1: ch += (unsigned char)*src++; ch <<= 6;
        case 0: ch += (unsigned char)*src++;
        }
        ch -= offsetsFromUTF8[nb];
        dest[i++] = ch;
    }
 done_toucs:
    dest[i] = 0;
    return i;
}

// Returns 0 if strings are different, 1 if they are the same
int compare_strings_u8(char * a, char * b)
{
	int a_len = u8_strlen(a);
	int b_len = u8_strlen(b);
	if (a_len != b_len) {
		return 0;
	};
	int i = 0;
	int j = 0;
	u_int32_t k = u8_nextchar(a, &i);
	u_int32_t l = u8_nextchar(b, &j);
	while (k == l && i < a_len) {
		if (!isutf(k)) return 0;
		if (k != l) return 0;
		k = u8_nextchar(a, &i);
		l = u8_nextchar(b, &j);
	}
	if (k != l || i != a_len) return 0;
	return 1;
}
