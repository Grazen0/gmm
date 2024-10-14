#ifndef LIB_PROCESSING
#define LIB_PROCESSING

#include <string>
#include <vector>

const int STATUS_OK = 0;
const int STATUS_ERR = 1;

int compile_from_ifstream(std::ifstream& src, std::vector<std::string>& output);

int process_line(const std::string& line, int& current_memory_address,
                 std::vector<int>& jmp_stack, std::vector<std::string>& output);

#endif
