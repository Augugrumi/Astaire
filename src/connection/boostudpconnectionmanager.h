#ifndef BOOSTUDPCONNECTIONMANAGER_H
#define BOOSTUDPCONNECTIONMANAGER_H

#include "udpconnectionmanager.h"

namespace connection {
class BoostUDPConnectionManager : public UDPConnectionManager
{
public:
    BoostUDPConnectionManager(boost::asio::io_service&, unsigned short int);

    void run();
    void stop();

    void send(const char*, std::function<void(const char*, int, std::size_t)>&);
    virtual void send(const char*);

private:
    std::unique_ptr<bip::udp::socket> socket;
    std::unique_ptr<boost::asio::io_service> service;
    bip::udp::endpoint endpoint;
    boost::array<char, 65536> buffer;

    void handle_message(const boost::system::error_code&, std::size_t);
};
}

#endif // BOOSTUDPCONNECTIONMANAGER_H
