#include "address.h"

namespace address {

const std::string Address::port_separator = ":";
const std::string Address::path_separator = "/";
const std::string Address::prefix = "http://";

Address::Address(const std::string& new_address, uint16_t new_port)
    : address(new_address), port(new_port), has_http_prefix(false) {
    if (strncmp(address.c_str(), prefix.c_str(), prefix.size()) == 0) {
        has_http_prefix = true;
    }
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
