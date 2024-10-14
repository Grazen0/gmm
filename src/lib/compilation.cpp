#include "compilation.h"
#include "matching.h"
#include <iostream>

// No sabemos bien qué es `basic_istream`, pero está en la firma de
// `std::getline`. Ya que leer tanto de `std::cin` como de un archivo usan
// `std::getline`, necesitábamos un tipo de parámetro que acepte ambas opciones.
//
// Un retorno `n != 0`, indica un error en la línea `n`.
int compile_from_istream(std::basic_istream<char>& src,
                         std::vector<std::string>& output)
{
    int line_number = 1;
    int mem_addr = 100;
    std::vector<int> jmp_stack;
    std::string line;

    while (std::getline(src, line))
    {
        int result = process_line(line, mem_addr, jmp_stack, output);

        if (result != STATUS_OK)
            return line_number;

        line_number++;
    }

    return STATUS_OK;
}

int process_line(const std::string& line, int& mem_addr,
                 std::vector<int>& jmp_stack, std::vector<std::string>& output)
{
    std::vector<std::string> match_args;

    if (match_trim("", line, match_args) == MATCH_OK)
    {
        // Just an empty line
    }
    else if (match_trim("while%s(%strue%s)%s{", line, match_args) == MATCH_OK)
    {
        int current_line = output.size();
        jmp_stack.push_back(current_line);
    }
    else if (match_trim("}", line, match_args) == MATCH_OK)
    {
        if (jmp_stack.empty())
            return STATUS_ERR;

        int jmp_dest = jmp_stack.back();
        jmp_stack.pop_back();

        output.push_back("jmp " + std::to_string(jmp_dest));
    }
    else if (match_statement("cin%s>>%s%r", line, match_args) == MATCH_OK)
    {
        std::string reg = match_args[0];

        output.push_back("in " + std::to_string(mem_addr));
        output.push_back("load " + reg + ", " + std::to_string(mem_addr));
        mem_addr++;
    }
    else if (match_statement("cout%s<<%s%r", line, match_args) == MATCH_OK)
    {
        std::string reg = match_args[0];

        output.push_back("store " + reg + ", " + std::to_string(mem_addr));
        output.push_back("out " + std::to_string(mem_addr));
        mem_addr++;
    }
    else if (match_statement("%r%s=%s%r", line, match_args) == MATCH_OK)
    {
        std::string dest = match_args[0];
        std::string src = match_args[1];

        output.push_back("mov " + src + ", " + dest);
    }
    else if (match_statement("%r%s=%s%i", line, match_args) == MATCH_OK)
    {
        std::string dest = match_args[0];
        std::string value = match_args[1];

        output.push_back("mvi " + dest + ", " + value);
    }
    else if (match_statement("%r%s=%s%r%s+%s%r", line, match_args) == MATCH_OK)
    {
        std::string dest = match_args[0];
        std::string left = match_args[1];
        std::string right = match_args[2];

        output.push_back("add " + dest + ", " + left + ", " + right);
    }
    else if (match_statement("%r%s=%s%r%s*%s%r", line, match_args) == MATCH_OK)
    {
        std::string dest = match_args[0];
        std::string left = match_args[1];
        std::string right = match_args[2];

        output.push_back("mul " + dest + ", " + left + ", " + right);
    }
    else if ((match_statement("%r%s++", line, match_args) == MATCH_OK) ||
             (match_statement("++%s%r", line, match_args) == MATCH_OK))
    {
        std::string reg = match_args[0];
        output.push_back("inc " + reg);
    }
    else if ((match_statement("%r%s--", line, match_args) == MATCH_OK) ||
             (match_statement("--%s%r", line, match_args) == MATCH_OK))
    {
        std::string reg = match_args[0];
        output.push_back("dec " + reg);
    }
    else
        return STATUS_ERR;

    return STATUS_OK;
}
