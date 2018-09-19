#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

namespace connection {
class ConnectionManager {
public:
    ConnectionManager() = default;
    virtual ~ConnectionManager() = default;

    virtual void run() = 0;
    virtual void stop() = 0;
};
}


#endif // CONNECTIONMANAGER_H
