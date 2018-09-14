#ifndef RAWSOCKETUDPCONNECTIONMANAGER_H
#define RAWSOCKETUDPCONNECTIONMANAGER_H

#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "udpconnectionmanager.h"
#include "log.h"
#include "asynctaskexecutor.h"

// FIXME for test purpose only
#include <chrono>
#include <atomic>
#include <iostream>
// END FIXME

#define TIMEOUT_WAIT -1 // No timeout

namespace connection {
class RawSocketUDPConnectionManager : public UDPConnectionManager
{
public:
    RawSocketUDPConnectionManager(unsigned short int);
    ~RawSocketUDPConnectionManager();

    void run();
    void stop();

    // TODO use promise for the async send?
    //void send(int, const char*, sockaddr* dest, std::function<void(const char*, int, std::size_t)>&);
    void send(int, const char*, sockaddr* dest);

private:
    struct pollfd pollfd;
    struct sockaddr_in addr;
    socklen_t addrlen;
    char* buf;
};
}


#endif // RAWSOCKETUDPCONNECTIONMANAGER_H
