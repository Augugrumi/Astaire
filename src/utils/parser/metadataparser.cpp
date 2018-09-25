#include "metadataparser.h"

MetadataParser::MetadataParser(const char* byte_array) noexcept {

    // FIXME: find correct way to calculate the length!
    unsigned int metadata_length = 42;/*strlen(METADATA_FLAG);*/

    char* circolar_buffer = new char[metadata_length];

    // TODO byte parsing. When parsed the metadata, convert it into char and
    // save it in the metadata string.

    delete[] circolar_buffer;
}
