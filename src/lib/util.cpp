#include "util.h"

bool is_numeric(const char ch)
{
    return '0' <= ch && ch <= '9';
}

bool is_valid_register(const char ch)
{
    return 'a' <= ch && ch <= 'z';
}
