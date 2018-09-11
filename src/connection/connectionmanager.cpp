#include "connectionmanager.h"

namespace connection {
ConnectionManager::ConnectionManager(
        const Pistache::Address& address,
        const PistacheConn::Endpoint::Options& server_options) {
    server = std::unique_ptr<PistacheConn::Endpoint>(
                new PistacheConn::Endpoint(address)
                );
    server->init(server_options);
}
}
