#include "lib/compilation.h"
#include <fstream>
#include <iostream>
#include <vector>

// `std::ifstream` nos permite leer archivos.
// Más adelante se usa con `std::getline` para leer línea por línea.
// https://www.geeksforgeeks.org/how-to-read-from-a-file-in-cpp/
std::ifstream prompt_file(const char prompt_message[])
{
    std::string filename;
    std::cout << prompt_message;
    std::cin >> filename;

    return std::ifstream(filename);
}

int handle_prompt_file_result(std::ifstream& input_file,
                              std::vector<std::string>& output)
{
    if (!input_file.is_open())
    {
        // Es más semántico usar `stderr` para mostrar los mensajes de error.
        // https://www.geeksforgeeks.org/cerr-standard-error-stream-object-in-cpp/
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return STATUS_ERR;
    }

    const int result = compile_from_ifstream(input_file, output);

    if (result == STATUS_UNMATCHED_BRACE)
    {
        std::cerr << "No se pudo compilar: bucle sin cerrar." << std::endl;
        return STATUS_ERR;
    }
    else if (result != STATUS_OK)
    {
        std::cerr << "No se pudo compilar: error en la línea " << result << "."
                  << std::endl;
        return STATUS_ERR;
    }

    return STATUS_OK;
}

int handle_compilation_result(const int compilation_result,
                              const std::vector<std::string>& output)
{
    if (compilation_result != STATUS_OK)
        return 1;

    for (auto& line : output)
        std::cout << line << std::endl;

    return 0;
}

int main()
{
    std::ifstream input_file = prompt_file("Nombre del archivo: ");

    std::vector<std::string> output;
    const int result = handle_prompt_file_result(input_file, output);

    input_file.close();

    return handle_compilation_result(result, output);
}
