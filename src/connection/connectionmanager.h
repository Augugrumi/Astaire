#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <memory>
#include <pistache/endpoint.h>
#include <pistache/router.h>

namespace connection {

namespace PConn = Pistache::Http;
namespace PReq = Pistache::Rest;

enum class RequestType {
    Get,
    Post,
    Put,
    Delete
};

class ConnectionManager {
public:
    ConnectionManager() = default;
    virtual ~ConnectionManager() = default;

    virtual void run() const = 0;
    virtual void stop() const = 0;

};
}


#endif // CONNECTIONMANAGER_H
