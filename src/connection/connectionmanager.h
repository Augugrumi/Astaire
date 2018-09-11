#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <memory>
#include <pistache/endpoint.h>
#include <pistache/router.h>

namespace connection {

namespace PConn = Pistache::Http;
namespace PReq = Pistache::Rest;

class ConnectionManager {
public:
    ConnectionManager(const Pistache::Address &,
                      const PConn::Endpoint::Options& options);

    ConnectionManager* addRoute(const std::string &, PReq::Route::Handler &);
    void run() const;
    void stop() const;

private:
    std::unique_ptr<PConn::Endpoint> server;
    std::unique_ptr<PReq::Router> router;
};
}


#endif // CONNECTIONMANAGER_H
