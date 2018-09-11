#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <memory>
#include "handler/handler.h"
#include <pistache/endpoint.h>

namespace connection {

namespace PistacheConn = Pistache::Http;

class ConnectionManager {
public:
    ConnectionManager(const Pistache::Address &,
                      const PistacheConn::Endpoint::Options& options);

    // FIXME - handler or route? :thinking:
    // ConnectionManager* addHandler(const PistacheConn::Handler &);
    void run() const;
    void stop() const;

private:
    std::unique_ptr<handler::Handler> handler;
    std::unique_ptr<PistacheConn::Endpoint> server;
};
}


#endif // CONNECTIONMANAGER_H
