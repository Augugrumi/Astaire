#ifndef HANDLER_H
#define HANDLER_H

#include <string>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <memory>
#include <functional>
#include <thread>

#include "../../utils/log.h"
#include "config.h"

#if HAS_BOOST_THREAD
#include <boost/asio/thread_pool.hpp>
#include <boost/asio.hpp>
#endif

namespace connection {
namespace handler {

namespace PConn = Pistache::Http;

// Empty class for future derivations (and language bindings)
class BindingHandler : public Pistache::Rest::Route::Handler{
public:
    BindingHandler();
    virtual ~BindingHandler() = default;

    virtual void onRequest(const PConn::Request&, PConn::ResponseWriter) = 0;

protected:
    std::string get_default_ack() const;
    void submit_task(const std::function<void()> &) const;

private:
#if HAS_BOOST_THREAD
    // FIXME In this way every instatiation has it's own thread pool!
    std::unique_ptr<boost::asio::thread_pool> thread_pool;
#endif
};
}
}

#endif // HANDLER_H
