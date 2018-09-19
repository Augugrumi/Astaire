#include "helloworldhandler.h"

#include "config.h"
#if HAS_TCP
namespace connection {
namespace handler {

void HelloWorldHandler::onRequest(const PConn::Request& request,
                                  PConn::ResponseWriter response) {
    LOG(ldebug, "Hello World called");

    auto async_task = [] (const std::string & payload) {
        LOG(ltrace, "Hello world lambda function. Printing the packet payload");
        LOG(ltrace, "------------BEGIN PACKET-------------");
        LOG(linfo, payload);
        LOG(ltrace, "-------------END PACKET--------------");
    };

    ASYNC_TASK(std::bind<void>(async_task, request.body()));

    LOG(ldebug, "Another print");

    response.send(Pistache::Http::Code::Ok, get_default_ack());
}
}
}

#endif