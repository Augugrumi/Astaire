#include "rawsocketudpconnectionmanager.h"


namespace connection {
RawSocketUDPConnectionManager::RawSocketUDPConnectionManager(
        uint32_t to_listen,
        unsigned short int port,
        handler::AbsHandler* abshandler) : UDPConnectionManager(port), handler(abshandler) {
    buf = new char[BUFFER_SIZE];

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(to_listen);
    addr.sin_port = htons(get_port());
}

RawSocketUDPConnectionManager::~RawSocketUDPConnectionManager() {
    delete buf;
}

void RawSocketUDPConnectionManager::run() {
    // Setting events for incoming data
    pollfd.fd = socket(AF_INET, SOCK_DGRAM, 0);
    pollfd.events = POLLIN;
    pollfd.revents = 0;

    LOG(ltrace, "File descriptor value" + std::to_string(pollfd.fd));

    if (pollfd.fd < 0) {
        LOG(lfatal, "Impossible to obtain a valid file descriptor");
        exit(1);
    }

    if (bind(
                pollfd.fd,
                (struct sockaddr*)&addr,
                sizeof(addr)) < 0) {
        LOG(lfatal, "Faliure binding to port: " + std::to_string(get_port()));
        exit(1);
    }

    std::atomic_int_fast64_t ct(0);

    // Main loop
    while (true) {
        /*
         * The first parameter, ufds, must point to an array of struct pollfd.
         * Each element in the array specifies a file descriptor that the
         * program is interested in monitoring, and what events on that file
         * descriptor the program would like to know about. The next parameter,
         * nfds, tells the kernel how many total items are in the ufds array.
         * The final parameter, timeout, is the maximum time, in milliseconds,
         * that the kernel should wait for the activities the ufds array
         * specifies. If timeout contains a negative value, the kernel will wait
         * forever. If nfds is zero, poll() becomes a simple millisecond sleep.
         */
        int poll_ret = poll(&pollfd, 1, TIMEOUT_WAIT);
        if (poll_ret > 0) {
            if (pollfd.revents & POLLIN) {
                LOG(ltrace, "Detected POLLIN event");

                sockaddr_in client;
                socklen_t clientlen = sizeof(client);
                ssize_t i = recvfrom(
                            pollfd.fd,
                            buf,
                            BUFFER_SIZE,
                            0,
                            (struct sockaddr*)&client,
                            &clientlen);

                if (i > 0) {
                    LOG(ltrace, "Data received");
                    auto packet_printer = [&ct, this] (char* buffer) {
                        unsigned  char* ubuffer = reinterpret_cast<unsigned char*>(buffer);
                        this->handler->handler_request(ubuffer, sizeof(ubuffer));
                        ct++;
                    };

                    ASYNC_TASK(std::bind<void>(packet_printer, buf));
                }
            }
        }
    }
}

void RawSocketUDPConnectionManager::stop() {

}

void RawSocketUDPConnectionManager::send(
        int fd,
        const char* message,
        sockaddr_in* dest,
        std::function<void(ssize_t)>& cb) {

    auto async_send = [this] (
            int fd,
            const char* message,
            sockaddr_in* dest,
            std::function <void(ssize_t)>& cb) {
        ssize_t res = send(fd, message, dest);
        cb(res);
    };

    ASYNC_TASK(std::bind<void>(async_send, fd, message, dest, cb));
}

ssize_t RawSocketUDPConnectionManager::send(
        int fd,
        const char* message,
        sockaddr_in* dest) {
    return sendto(
                fd,
                message,
                strlen(message),
                0,
                reinterpret_cast<struct sockaddr*>(dest),
                sizeof(*dest));
}

ssize_t RawSocketUDPConnectionManager::send(
        const char* message,
        const char* address,
        unsigned short int port) {

    // See https://linux.die.net/man/3/getaddrinfo
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    ssize_t res = -1;
    bool send_flag = true;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = IPPROTO_UDP; /* UDP protocol */

    s = getaddrinfo(address, std::to_string(port).c_str(), &hints, &result);

    if (s != 0) {
        LOG(lfatal, "Error getting info for: " +
            std::to_string(*address) +
            ". Error: " +
            std::to_string(*gai_strerror(s)))
        return res;
    }

    for (rp = result; rp != nullptr && send_flag; rp = rp->ai_next) {

        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd > 0) {
            res = send(
                        sfd,
                        message,
                        reinterpret_cast<struct sockaddr_in*>(rp->ai_addr));
            if (res > 0) {
                close(sfd);
                send_flag = false;
            }
        }
    }

    return res;
}

ssize_t RawSocketUDPConnectionManager::sound_send(
        int fd,
        const char* message,
        sockaddr_in* dest,
        short retr) {

    ssize_t result;

    while ((result = send(fd, message, dest)) < 0 && retr >= 0) {
        retr--;
    }

    if (retr < 0) {
        LOG(lwarn, "Failure when sending packet:\n" + std::to_string(*message));
        return -1;
    }

    return result;
}
}
