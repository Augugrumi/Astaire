#include "address.h"

namespace address {

const std::string Address::port_separator = ":";
const std::string Address::path_separator = "/";
const std::string Address::prefix = "http://";

Address::Address(const std::string& new_address, uint16_t new_port)
    : address(new_address), port(new_port), has_http_prefix(false) {
    has_http_prefix = prefix_check(new_address);
}

Address::Address(const std::string& complete_address) {
    has_http_prefix = prefix_check(complete_address);
    if (!has_http_prefix) {
        address_parse(complete_address);
    } else {
        size_t found = complete_address.find_first_of(path_separator);
        std::string protocol = complete_address.substr(0,found);
        std::string url_new = complete_address.substr(found+3);
        address_parse(url_new);
    }
}

void Address::address_parse(const std::string& url) {
    size_t found = url.find_first_of(port_separator);
    address = url.substr(0,found);
    size_t found1 = url.find_first_of(path_separator);
    port = std::stoul(url.substr(found+1,found1-found-1));
}

bool Address::prefix_check(const std::string& address) const {
    return strncmp(address.c_str(), prefix.c_str(), prefix.size()) == 0;
}

int Address::get_port() const {
    return port;
}

std::string Address::get_address() const {
    return address;
}

std::string Address::get_URL() const {
    std::string res;
    if (!has_http_prefix) {
        res.append(prefix);
    }
    res.append(address)
            .append(port_separator)
            .append(std::to_string(port))
            .append(path_separator);

    return res;
}
} // namespace address
