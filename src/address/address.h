#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <cstring>
#include <cstdint>

namespace address {

class Address
{
public:
    Address(const std::string&, uint16_t);

    const static std::string path_separator;

    int get_port() const;
    std::string get_address() const;

    std::string get_URL() const;
private:
    const static std::string prefix;
    const static std::string port_separator;
    std::string address;
    uint16_t port;
    bool has_http_prefix;
};
} // namespace address

#endif // ADDRESS_H
