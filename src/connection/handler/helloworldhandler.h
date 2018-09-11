#ifndef HELLOWORLDHANDLER_H
#define HELLOWORLDHANDLER_H

#include <pistache/async.h>
#include <thread>

#include "binding_handler.h"
#include "../../utils/log.h"

namespace connection {
namespace handler {
class HelloWorldHandler : public BindingHandler
{
public:
    HelloWorldHandler();
    virtual ~HelloWorldHandler() = default;

    void onRequest(const PConn::Request&, PConn::ResponseWriter);
};
}
}
#endif // HELLOWORLDHANDLER_H
