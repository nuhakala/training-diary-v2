// Add windows compatibility
#if defined(_WIN32)

#include <stdint.h>
#include <stdio.h>

typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;

ssize_t
getline(char **buf, size_t *bufsiz, FILE *fp);

#endif