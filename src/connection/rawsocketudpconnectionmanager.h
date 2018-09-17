#ifndef RAWSOCKETUDPCONNECTIONMANAGER_H
#define RAWSOCKETUDPCONNECTIONMANAGER_H

#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>

#include "udpconnectionmanager.h"
#include "log.h"
#include "asynctaskexecutor.h"
#include "config.h"

// FIXME for test purpose only
#include <chrono>
#include <atomic>
#include <iostream>
#include "handler/javahandler.h"
// END FIXME

//#include "handler/abshandler.h"

namespace connection {
class RawSocketUDPConnectionManager : public UDPConnectionManager
{
public:
    RawSocketUDPConnectionManager(uint32_t, unsigned short int, handler::AbsHandler*);
    ~RawSocketUDPConnectionManager();

    void run();
    void stop();

    // TODO use promise for the async send?
    void send(int, const char*, sockaddr_in*, std::function<void(ssize_t)>&);
    ssize_t send(int, const char*, sockaddr_in*);

    ssize_t sound_send(int, const char*, sockaddr_in* dest, short = 0);

private:
    struct pollfd pollfd;
    struct sockaddr_in addr;
    socklen_t addrlen;
    char* buf;
    handler::AbsHandler* handler;
};
}


#endif // RAWSOCKETUDPCONNECTIONMANAGER_H
