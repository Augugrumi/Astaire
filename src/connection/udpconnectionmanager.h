#ifndef UDPCONNECTIONMANAGER_H
#define UDPCONNECTIONMANAGER_H

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>

#include "handler/asynctaskexecutor.h"
#include "connectionmanager.h"
#include "log.h"

namespace bip = boost::asio::ip;

namespace connection {

class UDPConnectionManager : public ConnectionManager {
public:
    UDPConnectionManager(boost::asio::io_service&, unsigned short int);

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


#endif // UDPCONNECTIONMANAGER_H
