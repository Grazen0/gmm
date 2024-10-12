#include "util.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const int LINE_OK = 0;
const int LINE_ERR_UNMATCHED_BRACE = 1;
const int LINE_ERR_MISSING_SEMI = 2;
const int LINE_ERR_UNKOWN_INSTRUCTION = 3;

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

int process_line(std::string line, int& current_memory_address,
                 std::vector<int>& jmp_stack, std::vector<std::string>& output)
{
    trim_line(line);

    if (line.empty())
        return LINE_OK;

    if (line.rfind("while", 0) == 0)
    {
        // while(true) {
        int current_line = output.size();
        jmp_stack.push_back(current_line);
        return LINE_OK;
    }
    else if (line == "}")
    {
        // }
        if (jmp_stack.empty())
            return LINE_ERR_UNMATCHED_BRACE;

        int jmp_line = jmp_stack.back();
        jmp_stack.pop_back();
        output.push_back("jmp " + std::to_string(jmp_line));

        return LINE_OK;
    }
    else if (line.back() != ';')
    {
        return LINE_ERR_MISSING_SEMI;
    }
    else if (line.rfind("cin", 0) == 0)
    {
        // cin>>[reg];
        char reg = line[line.size() - 2];
        output.push_back("in " + std::to_string(current_memory_address));

        std::string load_line = "load ";
        load_line += reg;
        load_line += ", ";
        load_line += std::to_string(current_memory_address);
        output.push_back(load_line);

        current_memory_address++;
        return LINE_OK;
    }
    else if (line.rfind("cout", 0) == 0)
    {
        // cout<<[reg];
        char reg = line[line.size() - 2];

        output.push_back((std::string) "store " + reg + ", " +
                         std::to_string(current_memory_address));

        output.push_back("out " + std::to_string(current_memory_address));

        current_memory_address++;
        return LINE_OK;
    }
    else if (line.find("++") != std::string::npos)
    {
        // [reg]++;
        char reg = line[0];
        output.push_back((std::string) "inc " + reg);
        return LINE_OK;
    }
    else if (line.find("--") != std::string::npos)
    {
        // [reg]--;
        char reg = line[0];
        output.push_back((std::string) "dec " + reg);
        return LINE_OK;
    }
    else if (line[1] == '=')
    {
        // [reg]=[whatever]
        char dest_reg = line[0];

        if (line.size() == 4 && !is_numeric(line[2]))
        {
            // [reg]=[src_reg]
            char src_reg = line[2];
            output.push_back((std::string) "mov " + src_reg + ", " + dest_reg);
            return LINE_OK;
        }
        else if (is_numeric(line[2]))
        {
            // [reg]=[number]
            std::string value_str;

            for (int i = 2; is_numeric(line[i]); i++)
                value_str += line[i];

            output.push_back((std::string) "mvi " + dest_reg + ", " +
                             value_str);
            return LINE_OK;
        }
        else
        {
            // [reg]=[reg_a]+/*[reg_b]
            char dest = line[0];
            char a = line[2];
            char b = line[4];

            std::string instruction = line[3] == '+' ? "add " : "mul ";

            output.push_back(instruction + dest + ", " + a + ", " + b);
            return LINE_OK;
        }
    }

    return LINE_ERR_UNKOWN_INSTRUCTION;
}

int main()
{
    std::ifstream input_file("input.cpp");

    if (!input_file.is_open())
    {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    int line_number = 1;
    std::vector<std::string> output;
    int mem_addr = 100;
    std::vector<int> jmp_stack;
    std::string line;

    while (std::getline(input_file, line))
    {
        int result = process_line(line, mem_addr, jmp_stack, output);
        if (result != LINE_OK)
        {
            std::cerr << "Error en la línea " << line_number << ": "
                      << get_line_err_message(result) << std::endl;
            return 1;
        }

        line_number++;
    }

    input_file.close();

    for (auto& line : output)
        std::cout << line << std::endl;

    return 0;
}
