#ifndef RAWSOCKETUDPCONNECTIONMANAGER_H
#define RAWSOCKETUDPCONNECTIONMANAGER_H

#include "config.h"
#if HAS_UDP

#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <memory>
#include <atomic>

#include "udpconnectionmanager.h"
#include "log.h"
#include "asynctaskexecutor.h"
#include "handler/abshandler.h"
#include "stringutils.h"
#include "sfcheader/sfcfixedlengthheader.h"
#include "sfcheader/sfcutilities.h"
#include "addressresolver.h"
#include "address.h"

namespace sfcu = utils::sfc_header;

namespace connection {
class RawSocketUDPConnectionManager : public UDPConnectionManager
{
public:
    RawSocketUDPConnectionManager(
            uint32_t,
            unsigned short int,
            std::shared_ptr<handler::AbsHandler>,
            const address::Address&);
    ~RawSocketUDPConnectionManager();

    void run();
    void stop();

    // TODO use promise for the async send?
    void send(int,
              const char*,
              size_t,
              sockaddr_in*,
              std::function<void(ssize_t)>&) const;
    ssize_t send(int, const char*, size_t, sockaddr_in*) const;
    ssize_t send(const char*, size_t, const char*, unsigned short int) const;

    ssize_t sound_send(int, const char*, size_t, sockaddr_in*, short = 0);

    static void counter_printer(int);

    static std::atomic_int_fast64_t ct;
private:
    struct pollfd pollfd;
    struct sockaddr_in addr;

    address::Address roulette;
    socklen_t addrlen;
    char* buf;
    std::shared_ptr<handler::AbsHandler> handler;

    std::atomic_bool run_flag;

    void pkt_mngmt(ssize_t, msgptr);
};
}

#endif
#endif // RAWSOCKETUDPCONNECTIONMANAGER_H
