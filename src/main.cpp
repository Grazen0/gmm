#include "lib/processing.h"
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::string line;
    int line_number = 1;
    int mem_addr = 100;
    std::vector<int> jmp_stack;
    std::vector<std::string> output;

    while (std::getline(std::cin, line))
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

    for (auto& line : output)
        std::cout << line << std::endl;

    return 0;
}
