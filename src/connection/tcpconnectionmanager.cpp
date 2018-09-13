#include "tcpconnectionmanager.h"

namespace  connection {
TCPConnectionManager::TCPConnectionManager(
        const Pistache::Address& address,
        const PReq::Router & new_router,
        const PConn::Endpoint::Options& server_options)
    : server (std::unique_ptr<PConn::Endpoint>(new PConn::Endpoint(address))),
      router (std::unique_ptr<const PReq::Router>(&new_router)) {
    server->init(server_options);
    server->setHandler(router->handler());
}

void TCPConnectionManager::run() {
    server->serve();
}

void TCPConnectionManager::stop() {
    server->shutdown();
}

void TCPConnectionManager::send(const char* message,
                                std::function<void(const char*, int, std::size_t)> &callback) {

}

void TCPConnectionManager::send(const char* message) {

}
}

