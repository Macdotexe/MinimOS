#include "string.h"
#include <stdint.h>
#include <stddef.h>

const char* strchr(const char* str, char chr)
{
    if (str == NULL)
        return NULL;

    while (*str)
    {
        if (*str == chr)
            return str;

        ++str;
    }

    return NULL;
}

char* strcpy(char* dst, const char* src)
{
    char* origDst = dst;

    if (dst == NULL)
        return NULL;

    if (src == NULL)
    {
        *dst = '\0';
        return dst;
    }

    while (*src)
    {
        *dst = *src;
        ++src;
        ++dst;
    }
    
    *dst = '\0';
    return origDst;
}

unsigned strlen(const char* str)
{
    unsigned len = 0;
    while (*str)
    {
        ++len;
        ++str;
    }

    return len;
}

int atoi(const char *str, int* error) {
    int result = 0;
    int sign = 1;
    int i = 0;

    if (str == NULL) {
        return 0;
    }

    // Check for sign
    if (str[0] == '-' || str[0] == '+') {
        sign = (str[0] == '-') ? -1 : 1;
        i++;
    }

    // Convert the number
    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        } else {
            *error = 1;
            return 0;
        }
        i++;
    }

    return result * sign;
}
