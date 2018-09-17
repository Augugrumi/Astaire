#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <boost/asio.hpp>

#include "connection/tcpconnectionmanager.h"
#include "connection/boostudpconnectionmanager.h"
#include "connection/rawsocketudpconnectionmanager.h"
#include "connection/handler/helloworldhandler.h"
#include "utils/log.h"

int main()
{
#if DEBUG_BUILD
    utils::Log::instance()->set_log_level(utils::Log::Level::trace);
#endif
    LOG(linfo, "Astaire started");
    /*Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(9080));
    auto opts = Pistache::Http::Endpoint::options().threads(1);
    Pistache::Http::Endpoint server(addr);

    connection::handler::HelloWorldHandler* test = new connection::handler::HelloWorldHandler();
    auto bind = Pistache::Rest::Routes::bind(&connection::handler::HelloWorldHandler::onRequest, test);

    LOG(ldebug, "Handler created");

    Pistache::Rest::Router router = Pistache::Rest::Router();
    Pistache::Rest::Routes::Post(router, "/hello", bind);

    connection::TCPConnectionManager conn(addr, router, opts);
    LOG(ldebug, "Handler added");

    conn.run();*/

    /*boost::asio::io_service service;
    connection::BoostUDPConnectionManager conn(service, 8767);*/

    connection::RawSocketUDPConnectionManager conn(INADDR_ANY, 8767);

    conn.run();

    return 0;
}
