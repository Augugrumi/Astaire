#include <thread>

#include <csignal>
#include "connection/handler/handlercreator.h"
#include "connection/handler/abshandler.h"
#include "utils/log.h"
#include "log.h"
#include "connection/rawsocketudpconnectionmanager.h"
#include <memory>
#include <arpa/inet.h>
typedef std::shared_ptr<connection::handler::AbsHandler> handle_ptr;

namespace ch = connection::handler;

void usage() {
    const char message[] =
            "\n"
            "Example of usage:\n"
            "\t ./astaire [OPTIONS]\n"
            " -c         X  To set the path to the configure file, default './conf.json'\n"
            " -l            To specify the port on which the program wait for packets, default '8767'\n"
            " -i            To specify the IP address our roulette backend, default 'localhost'\n"
            " -f            To specify the port to which sends packets, default '8768'\n"
            " -r            Roulette address [address:port]"
            " -h            Show this message\n";
    std::cout <<message<<std::endl;
}

int main(int argc, char* argv[])
{
#if DEBUG_BUILD
    utils::Log::instance()->set_log_level(utils::Log::Level::trace);
#endif
    LOG(linfo, "Astaire started");

    std::string path = utils::HandlerFields::DEFAULT_CONFIG_PATH;
    unsigned short int listen_port = 8767;
    unsigned short int forward_port = 8768;
    std::string forward_address = "localhost:57684";

    int c;
    opterr = 0;
    while ((c = getopt(argc, (char **)argv, "c:uthl:i:f:r:")) != -1) {
        switch(c) {
            case 'c':
                if(optarg) {
                    path = optarg;
                }
                LOG(linfo, "Reading configuration file from " + path);
                break;
            case 'r':
                if(optarg) {
                    forward_address = optarg;
                }
                LOG(linfo, "Roulette address set to:" + forward_address);
                break;

            case 'l':
                if(optarg) {
                    listen_port = atoi(optarg);
                }
                LOG(linfo, "Listening on port: " +
                    std::to_string(htons(listen_port)));
                break;

            case 'i':
                if(optarg) {
                    forward_address = optarg;
                }
                LOG(linfo, "Setting forward address to " +
                    forward_address);
                break;

            case 'f':
                if(optarg) {
                    forward_port = atoi(optarg);
                }
                LOG(linfo, "Set forward port to: " +
                    std::to_string(htons(forward_port)));
                break;

            case 'h':
                usage();
                exit(0);
        }
    }
    LOG(linfo, "Opening UDP server");

    handle_ptr handler (
            ch::HandlerCreator::getHandlerByLanguageName(
                    utils::JsonUtils::JsonWrapper(path)
                    .getField(utils::HandlerFields::LAUNGUAGE), path)
                );

    auto conn = new connection::RawSocketUDPConnectionManager(INADDR_ANY,
                                                              listen_port,
                                                              handler,
                                                              forward_address);
    signal(SIGINT,
           connection::RawSocketUDPConnectionManager::counter_printer);

    conn->run();
    return 0;
}
