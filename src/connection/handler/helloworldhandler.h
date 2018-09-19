#ifndef HELLOWORLDHANDLER_H
#define HELLOWORLDHANDLER_H

#include "config.h"
#if HAS_TCP
#include <pistache/async.h>

#include <thread>

#include "binding_handler.h"
#include "log.h"
#include "asynctaskexecutor.h"

namespace connection {
namespace handler {
class HelloWorldHandler : public BindingHandler
{
public:
    HelloWorldHandler() = default;
    virtual ~HelloWorldHandler() = default;

    void onRequest(const PConn::Request&, PConn::ResponseWriter);
};
}
}
#endif

#endif // HELLOWORLDHANDLER_H
