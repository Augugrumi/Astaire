#include "asynctaskexecutor.h"

namespace connection {
namespace handler {

AsyncTaskExecutor* AsyncTaskExecutor::inst = new AsyncTaskExecutor();

AsyncTaskExecutor::AsyncTaskExecutor() noexcept {
#if HAS_BOOST_THREAD
    LOG(ldebug, "Using Boost Thread pool")
    thread_pool = std::unique_ptr<boost::asio::thread_pool>(
            new boost::asio::thread_pool(std::thread::hardware_concurrency()
    ));
#else
    LOG(ldebug, "Using fallback Thread pool")
    thread_pool = std::unique_ptr<utils::ThreadPool>(new utils::ThreadPool(std::thread::hardware_concurrency()));
#endif
}

AsyncTaskExecutor::~AsyncTaskExecutor() {
#if HAS_BOOST_THREAD
    thread_pool->stop();
#endif
    delete inst;
}

void AsyncTaskExecutor::submit_task(const std::function<void()> & task) const {
#if HAS_BOOST_THREAD
    boost::asio::post(*thread_pool, task);
#else
    thread_pool->enqueue(task);
#endif
}

AsyncTaskExecutor* AsyncTaskExecutor::instance() {
    return inst;
}
}
}
