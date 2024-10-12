#ifndef LIB_MATCHING
#define LIB_MATCHING

#include <string>
#include <vector>

const int MATCH_OK = 0;
const int MATCH_NOMATCH = 1;
const int MATCH_INVALID_PATTERN = 2;

int match_statement(const std::string& pattern, const std::string& src,
                    std::vector<std::string>& dest);

int match_trim(const std::string& pattern, const std::string& src,
               std::vector<std::string>& dest);

int match(const std::string& pattern, const std::string& src,
          std::vector<std::string>& dest);

#endif
