#include "udpconnectionmanager.h"

#include "config.h"

namespace connection {

UDPConnectionManager::UDPConnectionManager(unsigned short int listening_port) {
    port = listening_port;
}

uint16_t UDPConnectionManager::get_port() {
    return port;
}
}
