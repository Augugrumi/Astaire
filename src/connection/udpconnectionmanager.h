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
    UDPConnectionManager() = default;

};
}


#endif // UDPCONNECTIONMANAGER_H
