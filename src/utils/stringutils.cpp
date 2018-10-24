#include "stringutils.h"

// From https://stackoverflow.com/questions/9435385/split-a-string-using-c11
std::vector<std::string> utils::StringUtils::split (const std::string& input,
                                const std::string& token) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(token);
    std::sregex_token_iterator first{input.begin(), input.end(), re, -1}, last;
    return {first, last};
}