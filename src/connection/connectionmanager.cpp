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

ConnectionManager* ConnectionManager::addRoute(
        RequestType type_of_request,
        const std::string & route_path,
        PReq::Route::Handler & route) {

    // !! Passing unique_ptr router here
    switch (type_of_request) {
        case RequestType::Get : PReq::Routes::Get(*router, route_path, route); break;
        case RequestType::Post : PReq::Routes::Post(*router, route_path, route); break;
        case RequestType::Put : PReq::Routes::Put(*router, route_path, route); break;
        case RequestType::Delete : PReq::Routes::Delete(*router, route_path, route); break;
    }
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
