#include "matching.h"
#include "util.h"

int match_statement(const std::string& pattern, const std::string& src,
                    std::vector<std::string>& dest)
{
    return match("%s" + pattern + "%s;%s", src, dest);
}

int match_trim(const std::string& pattern, const std::string& src,
               std::vector<std::string>& dest)
{
    return match("%s" + pattern + "%s", src, dest);
}

// Pattern matching inspirado un poco en Rust y Haskell, con sintaxis
// basada en el formateo de `printf` en C.
// Toma un patrón y un string, y comprueba que el string coincide con el patrón.
// Además, extrae ciertos parámetros: nombres de registros y literales de
// `int`s.
//
// %r -> nombre de registro (se añade a match_args)
// %s -> cualquier cantidad de espaciado, posiblemente nada
// %i -> número entero, posiblemente con signo negativo (se añade a match_args)
// %% -> caracter '%' (no se usa, pero está implementado por completitud)
int match(const std::string& pattern, const std::string& src,
          std::vector<std::string>& match_args)
{
    int pattern_index = 0;
    int src_index = 0;

    match_args.clear();

    while (pattern_index < pattern.size())
    {
        char pattern_ch = pattern[pattern_index];

        if (pattern_ch != '%')
        {
            // Comparar caracteres tal cual
            if (src_index >= src.size() || pattern_ch != src[src_index])
                return MATCH_NOMATCH;

            pattern_index++;
            src_index++;
        }
        else
        {
            // Coincidencia de patrón
            if (pattern_index + 1 >= pattern.size())
                return MATCH_INVALID_PATTERN;

            char match_code = pattern[pattern_index + 1];

            switch (match_code)
            {
                case '%': { // Literal '%'
                    if (src_index >= src.size() || src[src_index] != '%')
                        return MATCH_NOMATCH;

                    src_index++;
                    break;
                }
                case 'r': { // Nombre de registro
                    if (src_index >= src.size())
                        return MATCH_NOMATCH;

                    char reg_char = src[src_index];
                    src_index++;

                    if (!is_valid_register(reg_char))
                        return MATCH_NOMATCH;

                    std::string reg_string(1, reg_char);
                    match_args.push_back(reg_string);
                    break;
                }
                case 'i': { // Número entero
                    if (src_index >= src.size())
                        return MATCH_NOMATCH;

                    std::string numeric_literal;

                    if (src[src_index] == '-')
                    {
                        numeric_literal += '-';
                        src_index++;
                    }

                    while (src_index < src.size() && is_numeric(src[src_index]))
                    {
                        numeric_literal += src[src_index];
                        src_index++;
                    }

                    if (numeric_literal.empty())
                        return MATCH_NOMATCH;

                    match_args.push_back(numeric_literal);
                    break;
                }
                case 's': { // Cualquier cantidad de espaciado
                    while (src_index < src.size() &&
                           (src[src_index] == ' ' || src[src_index] == '\t'))
                        src_index++;
                    break;
                }
                default:
                    return MATCH_INVALID_PATTERN;
            }

            pattern_index += 2;
        }
    }

    return src_index >= src.size() ? MATCH_OK : MATCH_NOMATCH;
}
