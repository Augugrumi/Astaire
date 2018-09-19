#ifndef ASYNCTASKEXECUTOR_H
#define ASYNCTASKEXECUTOR_H

#include <memory>
#include <functional>
#include <thread>

#include "config.h"
#include "log.h"

#if HAS_BOOST_THREAD
#include <boost/asio/thread_pool.hpp>
#include <boost/asio.hpp>
#else
#include "threadpool.h"
#endif


namespace connection {
namespace handler {
class AsyncTaskExecutor {
public:
    static AsyncTaskExecutor* instance();
    void submit_task(const std::function<void()> &) const;
protected:
    AsyncTaskExecutor() noexcept;
    virtual ~AsyncTaskExecutor();
private:
#if HAS_BOOST_THREAD
    std::unique_ptr<boost::asio::thread_pool> thread_pool;
#else
    std::unique_ptr<utils::ThreadPool> thread_pool;
#endif
    static AsyncTaskExecutor* inst;
};

#define ASYNC_TASK(task) \
    handler::AsyncTaskExecutor::instance()->submit_task(task);
}
}
#endif // ASYNCTASKEXECUTOR_H
