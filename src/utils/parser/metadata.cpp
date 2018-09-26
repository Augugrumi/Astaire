#include "metadata.h"


namespace utils {

namespace parser {

Metadata::Metadata(const unsigned char* byte_array) noexcept {

    // FIXME: find correct way to calculate the length!
    unsigned int metadata_length = 42;/*strlen(METADATA_FLAG);*/

    char* circolar_buffer = new char[metadata_length];

    // TODO byte parsing. When parsed the metadata, convert it into char and
    // save it in the metadata string.

    delete[] circolar_buffer;
}

std::string Metadata::get() {
    return metadata;
}
} // namespace parser

} // namespace utils
