#ifndef METADATA_H
#define METADATA_H

#include <memory>
#include <cstring>

#include "config.h"

namespace utils {

namespace parser {

class Metadata {
public:
    Metadata(const unsigned char*) noexcept;

    std::string get();

private:
    std::string metadata;
};
} // namespace parser

} // namespace utils

#endif // METADATA_H
