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
    Metadata(std::shared_ptr<unsigned char>) noexcept;

    std::string get_sfc_id();
    std::string get_sf_id();

private:
    std::string sf_id;
    std::string sfc_id;
};
} // namespace parser

} // namespace utils

#endif // METADATA_H
