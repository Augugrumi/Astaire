#include <csignal>

#include "connection/handler/handlercreator.h"
#include "connection/handler/abshandler.h"
#include "utils/log.h"
#include "log.h"

#if HAS_TCP
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <functional>
#include "connection/tcpconnectionmanager.h"
#include "connection/handler/helloworldhandler.h"
#endif

#if HAS_UDP
#include "connection/rawsocketudpconnectionmanager.h"
#include <memory>
typedef std::shared_ptr<connection::handler::AbsHandler> handle_ptr;
#endif

namespace ch = connection::handler;

void usage() {
    const char message[] =
            "\n"
            "Example of usage:\n"
            "\t ./astaire [OPTIONS]\n"
            " -c         X  To set the path to the configure file, default './conf.json'\n"
#if HAS_UDP
            " -u            To set connection type to UDP\n"
#endif
#if HAS_TCP
            " -t            To set connection type to TCP\n"
#endif
            " -l            To specify the port on which the program wait for packets, default '8767'\n"
            " -i            To specify the IP address to which the program sends packets, default 'localhost'\n"
            " -f            To specify the port to which sends packets, default '8768'\n"
            " -h            Show this message\n";
    std::cout <<message<<std::endl;
}

int main(int argc, char* argv[])
{
#if DEBUG_BUILD
    utils::Log::instance()->set_log_level(utils::Log::Level::trace);
#endif
    LOG(linfo, "Astaire started");

    std::string path = utils::JsonUtils::DEFAULT_CONFIG_PATH;
    unsigned short int listen_port = 8767;
    unsigned short int forward_port = 8768;
    std::string forward_address = "localhost";
#if (HAS_UDP and HAS_TCP)
    u_int8_t udp_flag = 0;
    u_int8_t tcp_flag = 0;
#elif HAS_UDP
    u_int8_t udp_flag = 1;
#elif HAS_TCP
    u_int8_t tcp_flag = 1;
#endif
    int c;
    opterr = 0;
    while ((c = getopt(argc, (char **)argv, "c:uthl:i:f:")) != -1) {
        switch(c) {
            case 'c':
                if(optarg) {
                    path = optarg;
                }
                break;
            case 'u':
#if HAS_UDP
                udp_flag = 1;
                break;
#else
                LOG(lfatal, "Astaire was not compile with UDP support");
                usage();
                exit(EXIT_FAILURE);
#endif
            case 't':
#if HAS_TCP
                tcp_flag = 1;
                break;
#else
                LOG(lfatal, "Astaire was not compile with TCP support");
                usage();
                exit(EXIT_FAILURE);
#endif
            case 'l':
                if(optarg) {
                    listen_port = atoi(optarg);
                }
                break;

            case 'i':
                if(optarg) {
                    forward_address = optarg;
                }
                break;

            case 'p':
                if(optarg) {
                    forward_port = atoi(optarg);
                }
                break;

            case 'h':
                usage();
                exit(0);
        }
    }

#if (HAS_UDP and HAS_TCP)
    if (udp_flag & tcp_flag) {
        LOG(lfatal, "Cannot set connection manager both to tcp and udp");
        usage();
        exit(EXIT_FAILURE);
    } else if (!(udp_flag | tcp_flag)) {
        udp_flag = 1;
    }
#endif
#if HAS_UDP
    if (udp_flag) {

        LOG(linfo, "Opening UDP server");

        handle_ptr handler (
                ch::HandlerCreator::getHandlerByLanguageName(
                        utils::JsonUtils::JsonWrapper(path)
                        .getField(utils::JsonUtils::LAUNGUAGE), path)
                    );


        connection::RawSocketUDPConnectionManager conn(INADDR_ANY,
                                                       listen_port,
                                                       forward_address,
                                                       forward_port,
                                                       handler);
        signal(SIGINT,
               connection::RawSocketUDPConnectionManager::counter_printer);

        conn.run();
    }
#endif
#if HAS_TCP
    if (tcp_flag) {

        LOG(linfo, "Opening TCP server");

        Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(9080));
        auto opts = Pistache::Http::Endpoint::options().threads(1);
        Pistache::Http::Endpoint server(addr);

        ch::HelloWorldHandler* test = new ch::HelloWorldHandler();
        auto bind = Pistache::Rest::Routes::bind(
                    &ch::HelloWorldHandler::onRequest,
                    test);

        Pistache::Rest::Router router = Pistache::Rest::Router();
        Pistache::Rest::Routes::Post(router, "/hello", bind);

        connection::TCPConnectionManager conn(addr, router, opts);

        conn.run();
    }
#endif
    return 0;
}
