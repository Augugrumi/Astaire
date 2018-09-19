#ifndef HANDLER_H
#define HANDLER_H

#include "config.h"
#if HAS_TCP

#include <string>
#include <pistache/endpoint.h>
#include <pistache/router.h>

#include "log.h"

namespace connection {
namespace handler {

namespace PConn = Pistache::Http;

// Empty class for future derivations (and language bindings)
class BindingHandler : public Pistache::Rest::Route::Handler {
public:
    BindingHandler() = default;
    virtual ~BindingHandler() = default;

    virtual void onRequest(const PConn::Request&, PConn::ResponseWriter) = 0;

protected:
    std::string get_default_ack() const;
};
}
}

#endif

#endif // HANDLER_H
