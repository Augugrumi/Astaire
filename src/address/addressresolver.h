#ifndef ADDRESSRESOLVER_H
#define ADDRESSRESOLVER_H

#include <string>
#include <memory>
#include <functional>
#include <curl/curl.h>
#include <map>
#include <vector>
#include <linux/ip.h>

#include "sfcheader/sfcfixedlengthheader.h"
#include "address.h"
#include "log.h"
#include "jsonutils.h"
#include "maprefresher.h"

namespace address {

class AddressResolver {
/*
 * TODO create a map<sfcid/chain> that autorefreshes each some
 * interval of time
 */

public:
    AddressResolver(const std::string&, uint16_t port);
    AddressResolver(const Address&);

    const Address get_next(uint32_t, uint32_t,
                           utils::sfc_header::SFCFixedLengthHeader,
                           unsigned char* pkt) const;

    virtual ~AddressResolver();
private:
        const Address roulette_addr;
        CURL* curl = nullptr;
    std::map<uint32_t, std::vector<Address>> local_resolver;
    refresher::map_refresher<uint32_t, std::vector<Address>>* updater;

    const std::string url_builder(const std::string&, uint32_t, uint32_t) const;
    static size_t curl_callback(void*, size_t, size_t, std::string*);

    void setup_curl_for_request(std::string req_addr, std::string& req_data_res) const;

    const Address get_chain_endpoint(utils::sfc_header::SFCFixedLengthHeader,
                                     unsigned char* pkt) const;
};
} // namespace address

#endif // ADDRESSRESOLVER_H
