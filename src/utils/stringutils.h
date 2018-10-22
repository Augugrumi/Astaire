#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <vector>
#include <regex>
#include <string>

namespace utils {

class StringUtils
{
public:
    static std::vector<std::string> split(const std::string&, const std::string&);
private:
    StringUtils() = default;
};
} // namespace utils


#endif // STRINGUTILS_H
