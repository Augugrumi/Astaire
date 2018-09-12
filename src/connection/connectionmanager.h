#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <memory>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <functional>

namespace connection {
class ConnectionManager {
public:
    ConnectionManager() = default;
    virtual ~ConnectionManager() = default;

    virtual void run() = 0;
    virtual void stop() = 0;

    virtual void send(const char*, std::function<void(const char*, int, std::size_t)> &) = 0;
    virtual void send(const char*) = 0;

};
}


#endif // CONNECTIONMANAGER_H
