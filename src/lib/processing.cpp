#include "processing.h"
#include "matching.h"

std::string get_line_err_message(const int err)
{
    switch (err)
    {
        case LINE_ERR_UNMATCHED_BRACE:
            return "Llave de cierre sin emparejar";
        case LINE_ERR_MISSING_SEMI:
            return "Punto y coma faltante";
        case LINE_ERR_UNKOWN_INSTRUCTION:
            return "Instrucción no reconocida";
        default:
            return "Error desconocido";
    }
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
            return LINE_ERR_UNMATCHED_BRACE;

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
    else if (match_statement("%r%s++", line, match_args) == MATCH_OK)
    {
        std::string reg = match_args[0];
        output.push_back("inc " + reg);
    }
    else if (match_statement("%r%s--", line, match_args) == MATCH_OK)
    {
        std::string reg = match_args[0];
        output.push_back("dec " + reg);
    }
    else
        return LINE_ERR_UNKOWN_INSTRUCTION;

    return LINE_OK;
}
