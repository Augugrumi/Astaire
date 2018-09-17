#include "rawsocketudpconnectionmanager.h"

namespace connection {
RawSocketUDPConnectionManager::RawSocketUDPConnectionManager(
        uint32_t to_listen,
        unsigned short int port) : UDPConnectionManager(port) {
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
                reinterpret_cast<struct sockaddr *>(&addr),
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
                            reinterpret_cast<struct sockaddr *>(&client),
                            &clientlen);

                if (i > 0) {
                    LOG(ltrace, "Data received");
                    auto packet_printer = [&ct] (char* buffer) {
                        std::cout<<ct<<std::endl;
                        ct++;
                    };

                    send(pollfd.fd, "ACK", &client);
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
        const char* message, sockaddr_in* dest) {
    return sendto(
                fd,
                message,
                strlen(message),
                0,
                reinterpret_cast<struct sockaddr*>(dest),
                sizeof(*dest));
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
