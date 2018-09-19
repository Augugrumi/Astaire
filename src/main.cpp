#include <pistache/router.h>
#include <boost/asio.hpp>
#include <csignal>

#include "connection/handler/handlercreator.h"
#include "connection/handler/abshandler.h"
#include "utils/log.h"
#include "log.h"
#if HAS_TCP
/*#include <pistache/endpoint.h>
#include <functional>*/
//#include "connection/tcpconnectionmanager.h"
//#include "connection/boostudpconnectionmanager.h"
#endif
#if HAS_UDP
#include "connection/rawsocketudpconnectionmanager.h"
#endif


int main(int argc, char* argv[])
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

    std::string path = utils::JsonUtils::DEFAULT_CONFIG_PATH;

    int c ;
    opterr = 0;
    while ((c = getopt(argc, (char **)argv, "c:")) != -1) {
        switch(c) {
            case 'c':
                if(optarg) {
                    path = optarg;
                }
                break;
        }
    }

    connection::handler::AbsHandler* handler =
            connection::handler::HandlerCreator::getHandlerByLanguageName(
                    utils::JsonUtils::JsonWrapper(path).getField(utils::JsonUtils::LAUNGUAGE), path);

    connection::RawSocketUDPConnectionManager conn(INADDR_ANY, 8767, handler);
    signal(SIGINT, connection::RawSocketUDPConnectionManager::counter_printer);

    conn.run();

    return 0;
}
