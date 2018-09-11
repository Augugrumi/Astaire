#include <iostream>

#include <pistache/endpoint.h>
#include "utils/log.h"

class HelloHandler : public Pistache::Http::Handler {
public:

    HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
        utils::Log::instance()->log(utils::Log::Level::trace, "Message arrived");
        response.send(Pistache::Http::Code::Ok, "Hello, World");
    }
};


int main()
{
    utils::Log::instance()->log(utils::Log::Level::debug, "Program started");
    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(9080));
    auto opts = Pistache::Http::Endpoint::options().threads(1);
    Pistache::Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(std::make_shared<HelloHandler>());
    server.serve();
    return 0;
}
