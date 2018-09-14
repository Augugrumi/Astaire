#ifndef LINUXSOCKETUDPCONNECTIONMANAGER_H
#define LINUXSOCKETUDPCONNECTIONMANAGER_H


#include "connectionmanager.h"

namespace connection {
class LinuxSocketUDPConnectionManager : public ConnectionManager
{
public:
    LinuxSocketUDPConnectionManager();
};
}


#endif // LINUXSOCKETUDPCONNECTIONMANAGER_H
