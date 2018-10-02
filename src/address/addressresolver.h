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
    AddressResolver(const std::string&, uint16_t port);
    AddressResolver(const Address&);
    virtual ~AddressResolver();

    const Address get_next(uint32_t, uint32_t) const;
private:
    const Address roulette_addr;
    CURL* curl = nullptr;

    const std::string url_builder(const std::string&, uint32_t, uint32_t) const;
    static size_t curl_callback(void*, size_t, size_t, std::string*);
};
} // namespace address

#endif // ADDRESSRESOLVER_H
