#include "connectionmanager.h"

namespace connection {
ConnectionManager::ConnectionManager(
        const Pistache::Address& address,
        const PConn::Endpoint::Options& server_options) {
    server = std::unique_ptr<PConn::Endpoint>(
                new PConn::Endpoint(address)
                );

    server->init(server_options);

    router = std::unique_ptr<PReq::Router>(
                new PReq::Router()
                );
}

ConnectionManager* ConnectionManager::addRoute(const std::string & route_path,
                                               PReq::Route::Handler & route) {

    // !! Passing unique_ptr router here
    PReq::Routes::Get(*router, route_path, route);
    return this;
}

void ConnectionManager::run() const {
    server->setHandler(router->handler());

    server->serve();
}

void ConnectionManager::stop() const {
    server->shutdown();
}
}
