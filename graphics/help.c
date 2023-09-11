#include "help.h"
extern int printf(char*, ...);
int error;
double str_to_num(const char* start, const char* end)
{
    end--;
    _Bool zap = 0;
    double before = 0, after = 0;
    for (; start <= end && (*start); start++)
        if ((*start) == '.' || (*start) == ',')
        {
            zap = 1; start++; break;
        }
        else { before *= 10; before += ((*start) - '0'); }

    if (zap)for (; end >= start; end--)
    {
        after += ((*end) - '0'); after /= 10;
    }
    return before + after;
}

_Bool fract_str_cmp_NR(const char* str1, const char* str2, int num)
{
    while (num) {
        if (((*str1) | 0b100000) != ((*str2) | 0b100000)) return 0;
        str1++; str2++;
        num--;
    }
    return 1;
}