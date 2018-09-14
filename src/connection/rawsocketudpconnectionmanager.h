#ifndef RAWSOCKETUDPCONNECTIONMANAGER_H
#define RAWSOCKETUDPCONNECTIONMANAGER_H

#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "udpconnectionmanager.h"
#include "log.h"
#include "asynctaskexecutor.h"

#define BUFF_SIZE 1024
#define TIMEOUT_WAIT -1

namespace connection {
class RawSocketUDPConnectionManager : public UDPConnectionManager
{
public:
    RawSocketUDPConnectionManager(unsigned short int);
    ~RawSocketUDPConnectionManager();

    void run();
    void stop();

    void send(const char*, std::function<void(const char*, int, std::size_t)>&);
    void send(const char*);

private:
    struct pollfd pollfd;
    struct sockaddr_in addr;
    socklen_t addrlen;
    char* buf;
};
}


#endif // RAWSOCKETUDPCONNECTIONMANAGER_H
