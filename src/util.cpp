#include <string>

bool is_numeric(char ch)
{
    return '0' <= ch && ch <= '9';
}

// TODO: validar nombres de registros usando esto
bool is_valid_register(char ch)
{
    return 'a' <= ch && ch <= 'z';
}

void trim_line(std::string& s)
{
    while (s[0] == ' ' || s[0] == '\t')
        s.erase(s.begin());

    while (s.back() == ' ' || s.back() == '\t')
        s.erase(s.end() - 1);
}
