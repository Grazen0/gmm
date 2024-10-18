#include "lib/compilation.h"
#include <fstream>
#include <iostream>
#include <vector>

// `std::ifstream` nos permite leer archivos.
// Más adelante se usa con `std::getline` para leerlo línea por línea.
// https://www.geeksforgeeks.org/how-to-read-from-a-file-in-cpp/
std::ifstream prompt_file(const char prompt_message[])
{
    std::string filename;
    std::ifstream stream;

    do
    {
        std::cout << prompt_message;
        std::cin >> filename;
        stream = std::ifstream(filename);
    } while (!stream.is_open());

    return stream;
}

int handle_compilation_result(const int result,
                              const std::vector<std::string>& output)
{
    if (result == STATUS_UNMATCHED_BRACE)
    {
        // Es más semántico usar `stderr` para mostrar los mensajes de error.
        // https://www.geeksforgeeks.org/cerr-standard-error-stream-object-in-cpp/
        std::cerr << "No se pudo compilar: bucle sin cerrar." << std::endl;
        return 1;
    }
    else if (result != STATUS_OK)
    {
        std::cerr << "No se pudo compilar: error en la línea " << result << "."
                  << std::endl;
        return 1;
    }

    std::cout << "Resultado de compilación:" << std::endl;

    for (auto& line : output)
        std::cout << line << std::endl;

    return 0;
}

int main()
{
    std::ifstream input_file = prompt_file("Nombre del archivo: ");

    std::vector<std::string> output;
    const int result = compile_from_ifstream(input_file, output);

    input_file.close();

    return handle_compilation_result(result, output);
}
