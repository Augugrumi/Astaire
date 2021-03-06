#ifndef UDPCONNECTIONMANAGER_H
#define UDPCONNECTIONMANAGER_H

#include "config.h"

#include <cstdint>
#include <netinet/in.h>

#include "connectionmanager.h"

namespace connection {

class UDPConnectionManager : public ConnectionManager {
public:
    UDPConnectionManager(unsigned short int);

protected:
    uint16_t get_port();

private:
    uint16_t port;

};

}

#endif // UDPCONNECTIONMANAGER_H
