#include "helloworldhandler.h"

namespace connection {
namespace handler {
HelloWorldHandler::HelloWorldHandler()
{

}

void HelloWorldHandler::onRequest(const PConn::Request& request,
                                  PConn::ResponseWriter response) {
    LOG(ldebug, "Hello World called");

    auto async_task = [] () {
        LOG(ltrace, "In the async task");
    };

    ASYNC_TASK(async_task);

    LOG(ldebug, "Another print");

    response.send(Pistache::Http::Code::Ok, get_default_ack());
}
}
}
