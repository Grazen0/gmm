#include "lib/compilation.h"
#include <fstream>
#include <iostream>
#include <vector>

int compile_from_argv(const int argc, const char* argv[],
                      std::vector<std::string>& output)
{
    if (argc < 2)
    {
        // Esta página explica cómo leer de `stdin` hasta un carácter EOF (fin
        // de archivo).
        // https://stackoverflow.com/questions/201992/how-to-read-until-eof-from-cin-in-c
        return compile_from_istream(std::cin, output);
    }
    else
    {
        // Esta página explica cómo leer un archivo línea por línea.
        // https://www.geeksforgeeks.org/how-to-read-from-a-file-in-cpp/
        std::ifstream input_file(argv[1]);

        if (!input_file.is_open())
        {
            // Es más semántico usar `stderr` para mostrar los mensajes de
            // error.
            // https://www.geeksforgeeks.org/cerr-standard-error-stream-object-in-cpp/
            std::cerr << "No se pudo abrir el archivo." << std::endl;
            return 1;
        }

        return compile_from_istream(input_file, output);
    }
}

int handle_compilation_result(const int compilation_result,
                              const std::vector<std::string>& output)
{
    if (compilation_result != STATUS_OK)
    {
        std::cerr << "Error en la línea " << compilation_result << "."
                  << std::endl;
        return 1;
    }

    for (auto& line : output)
        std::cout << line << std::endl;

    return 0;
}

// Esta página explica cómo tomar argumentos de la terminal.
// https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/
// Es lo que nos permite tomar opcionalmente el nombre de un archivo.
int main(const int argc, const char* argv[])
{
    std::vector<std::string> output;
    int result = compile_from_argv(argc, argv, output);
    return handle_compilation_result(result, output);
}
