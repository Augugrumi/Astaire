#include "metadata.h"


namespace utils {

namespace parser {

Metadata::Metadata(const unsigned char* byte_array) noexcept {

    // FIXME: find correct way to calculate the length!
    unsigned int metadata_length = 42;/*strlen(METADATA_FLAG);*/

    char* circolar_buffer = new char[metadata_length];

    // TODO byte parsing. When parsed the metadata, convert it into char and
    // save it in sf_id and sfc_id, and save the header in metadata.

    delete[] circolar_buffer;
}

Metadata::Metadata(std::shared_ptr<unsigned char> byte_array) noexcept
    : Metadata(byte_array.get()) {
}

std::string Metadata::get_sf_id() {
    return sf_id;
}

std::string Metadata::get_sfc_id() {
    return sfc_id;
}

std::string Metadata::get_header() {
    return header;
}

} // namespace parser

} // namespace utils
