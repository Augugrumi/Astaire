#ifndef METADATAPARSER_H
#define METADATAPARSER_H

#include <memory>
#include <cstring>

#include "config.h"

class MetadataParser
{
public:
    MetadataParser(const char*) noexcept;

private:
    std::string metadata;
};

#endif // METADATAPARSER_H
