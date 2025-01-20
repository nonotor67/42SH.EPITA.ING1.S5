#include "utils.h

char *my_itoa(int value, char *s)
{
    if (value == 0)
    {
        s[0] = '0';
        s[1] = '\0';
        return s;
    }
    size_t i = 0;
    if (value < 0)
    {
        s[i] = '-';
        i++;
        value = -value;
    }
    long value_rev = 0;
    int nb_zero = 0;
    while (value != 0)
    {
        if (value % 10 == 0 && value_rev == 0)
            nb_zero++;
        value_rev *= 10;
        value_rev += value % 10;
        value /= 10;
    }
    for (; value_rev != 0; i++)
    {
        s[i] = value_rev % 10 + '0';
        value_rev /= 10;
    }
    for (; nb_zero > 0; i++)
    {
        s[i] = '0';
        nb_zero--;
    }
    s[i] = '\0';
    return s;
}
