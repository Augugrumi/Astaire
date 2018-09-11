#include "asynctaskexecutor.h"

namespace connection {
namespace handler {

AsyncTaskExecutor* AsyncTaskExecutor::inst = nullptr;

AsyncTaskExecutor::AsyncTaskExecutor() noexcept {
#if HAS_BOOST_THREAD
    LOG(ldebug, "Using Boost Thread pool")
    thread_pool = std::unique_ptr<boost::asio::thread_pool>(
            new boost::asio::thread_pool(std::thread::hardware_concurrency()
    ));
#endif
}

AsyncTaskExecutor::~AsyncTaskExecutor() {
    thread_pool->stop();
    delete inst;
}

void AsyncTaskExecutor::submit_task(const std::function<void()> & task) const {
#if HAS_BOOST_THREAD
    boost::asio::post(*thread_pool, task);
#else
    std::thread todo_task(task);
    todo_task.detach();
#endif
}

AsyncTaskExecutor* AsyncTaskExecutor::instance() {
    if (inst == nullptr) {
        inst = new AsyncTaskExecutor();
    }
    return inst;
}
}
}
