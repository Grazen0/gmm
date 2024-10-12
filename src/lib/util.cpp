#include "util.h"

bool is_numeric(char ch)
{
    return '0' <= ch && ch <= '9';
}

bool is_valid_register(char ch)
{
    return 'a' <= ch && ch <= 'z';
}
