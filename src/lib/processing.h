#ifndef LIB_PROCESSING
#define LIB_PROCESSING

#include <string>
#include <vector>

const int LINE_OK = 0;
const int LINE_ERR_UNMATCHED_BRACE = 1;
const int LINE_ERR_UNKOWN_INSTRUCTION = 2;

std::string get_line_err_message(const int err);

int process_line(const std::string& line, int& current_memory_address,
                 std::vector<int>& jmp_stack, std::vector<std::string>& output);

#endif
