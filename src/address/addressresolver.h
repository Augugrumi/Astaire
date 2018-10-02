#ifndef ADDRESSRESOLVER_H
#define ADDRESSRESOLVER_H

#include <string>
#include <memory>
#include <functional>
#include <curl/curl.h>

#include "address.h"
#include "log.h"
#include "jsonutils.h"

namespace address {

class AddressResolver
{
public:
    AddressResolver(const std::string&, int port);
    AddressResolver(const Address&);
    virtual ~AddressResolver();

    const Address get_next(uint32_t, uint32_t) const;
private:
    const Address roulette_addr;
    CURL* curl;

    const std::string url_builder(const std::string&, uint32_t, uint32_t) const;
};
} // namespace address

#endif // ADDRESSRESOLVER_H
