#include "binding_handler.h"

namespace connection {
namespace handler {
BindingHandler::BindingHandler() : Pistache::Rest::Route::Handler () {
#if HAS_BOOST_THREAD
    LOG(ldebug, "Using Boost Thread pool")
    thread_pool = std::unique_ptr<boost::asio::thread_pool>(
            new boost::asio::thread_pool(std::thread::hardware_concurrency()
    ));
#endif
    LOG(ldebug, "Not using boost")
}

std::string BindingHandler::get_default_ack() const {
    return "ACK";
}

void BindingHandler::submit_task(const std::function<void()> & task) const {
#if HAS_BOOST_THREAD
    boost::asio::post(*thread_pool, task);
#else
    std::thread todo_task(task);
    todo_task.detach();
#endif
}
}
}
