#include <pistache/endpoint.h>
#include <pistache/router.h>

#include "connection/tcpconnectionmanager.h"
#include "connection/handler/helloworldhandler.h"
#include "utils/log.h"


int main()
{
    LOG(ldebug, "Program started");
    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(9080));
    auto opts = Pistache::Http::Endpoint::options().threads(1);
    Pistache::Http::Endpoint server(addr);

    connection::handler::HelloWorldHandler* test = new connection::handler::HelloWorldHandler();
    auto bind = Pistache::Rest::Routes::bind(&connection::handler::HelloWorldHandler::onRequest, test);

    LOG(ldebug, "Handler created");

    Pistache::Rest::Router router = Pistache::Rest::Router();
    Pistache::Rest::Routes::Post(router, "/hello", bind);

    connection::TCPConnectionManager conn(addr, router, opts);
    LOG(ldebug, "Handler added");

    conn.run();

    return 0;
}
