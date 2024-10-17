#include "compilation.h"
#include "matching.h"
#include <fstream>

// Valores de retorno
// n =  0  -> Ok
// n = -1  -> Bucle sin cerrar
// n >  0  -> Error de sintaxis en la línea `n`
int compile_from_ifstream(std::ifstream& src, std::vector<std::string>& output)
{
    int line_number = 1;
    int mem_addr = 100;
    std::vector<int> jmp_stack;
    std::string line;

    while (std::getline(src, line))
    {
        const int result = process_line(line, mem_addr, jmp_stack, output);

        if (result != STATUS_OK)
            return line_number;

        line_number++;
    }

    if (!jmp_stack.empty())
        return STATUS_UNMATCHED_BRACE;

    return STATUS_OK;
}

int process_line(const std::string& line, int& mem_addr,
                 std::vector<int>& jmp_stack, std::vector<std::string>& output)
{
    std::vector<std::string> match_args;

    if (match("%s", line, match_args) == MATCH_OK)
    {
        // Línea vacía
    }
    else if (match_trim("while%s(%strue%s)%s{", line, match_args) == MATCH_OK)
    {
        const int current_line = output.size();
        jmp_stack.push_back(current_line);
    }
    else if (match_trim("}", line, match_args) == MATCH_OK)
    {
        if (jmp_stack.empty())
            return STATUS_ERR;

        const int jmp_dest = jmp_stack.back();
        jmp_stack.pop_back();

        output.push_back("jmp " + std::to_string(jmp_dest));
    }
    else if (match_statement("cin%s>>%s%r", line, match_args) == MATCH_OK)
    {
        const std::string reg = match_args[0];

        output.push_back("in " + std::to_string(mem_addr));
        output.push_back("load " + reg + ", " + std::to_string(mem_addr));
        mem_addr++;
    }
    else if (match_statement("cout%s<<%s%r", line, match_args) == MATCH_OK)
    {
        const std::string reg = match_args[0];

        output.push_back("store " + reg + ", " + std::to_string(mem_addr));
        output.push_back("out " + std::to_string(mem_addr));
        mem_addr++;
    }
    else if (match_statement("%r%s=%s%r", line, match_args) == MATCH_OK)
    {
        const std::string dest = match_args[0];
        const std::string src = match_args[1];

        output.push_back("mov " + src + ", " + dest);
    }
    else if (match_statement("%r%s=%s%i", line, match_args) == MATCH_OK)
    {
        const std::string dest = match_args[0];
        const std::string value = match_args[1];

        output.push_back("mvi " + dest + ", " + value);
    }
    else if (match_statement("%r%s=%s%r%s+%s%r", line, match_args) == MATCH_OK)
    {
        const std::string dest = match_args[0];
        const std::string left = match_args[1];
        const std::string right = match_args[2];

        output.push_back("add " + dest + ", " + left + ", " + right);
    }
    else if (match_statement("%r%s=%s%r%s*%s%r", line, match_args) == MATCH_OK)
    {
        const std::string dest = match_args[0];
        const std::string left = match_args[1];
        const std::string right = match_args[2];

        output.push_back("mul " + dest + ", " + left + ", " + right);
    }
    else if ((match_statement("%r%s++", line, match_args) == MATCH_OK) ||
             (match_statement("++%s%r", line, match_args) == MATCH_OK))
    {
        const std::string reg = match_args[0];
        output.push_back("inc " + reg);
    }
    else if ((match_statement("%r%s--", line, match_args) == MATCH_OK) ||
             (match_statement("--%s%r", line, match_args) == MATCH_OK))
    {
        const std::string reg = match_args[0];
        output.push_back("dec " + reg);
    }
    else
        return STATUS_ERR;

    return STATUS_OK;
}
