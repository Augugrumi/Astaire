#ifndef RAWSOCKETUDPCONNECTIONMANAGER_H
#define RAWSOCKETUDPCONNECTIONMANAGER_H

#include "udpconnectionmanager.h"

namespace connection {
class RawSocketUDPConnectionManager : public UDPConnectionManager
{
public:
    RawSocketUDPConnectionManager(unsigned short int);
};
}


#endif // RAWSOCKETUDPCONNECTIONMANAGER_H
