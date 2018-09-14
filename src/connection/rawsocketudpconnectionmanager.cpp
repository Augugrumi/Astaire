#include "rawsocketudpconnectionmanager.h"

namespace connection {
RawSocketUDPConnectionManager::RawSocketUDPConnectionManager(unsigned short int port)
    : UDPConnectionManager(port) {
}
}
