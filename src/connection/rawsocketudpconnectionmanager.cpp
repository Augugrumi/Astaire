#include "rawsocketudpconnectionmanager.h"

namespace connection {
RawSocketUDPConnectionManager::RawSocketUDPConnectionManager(unsigned short int port)
    : UDPConnectionManager(port) {
    buf = new char[BUFF_SIZE];

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
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

    std::string t = "File descriptor value: ";
    t.append(std::to_string(pollfd.fd));
    LOG(ldebug, t);

    if (pollfd.fd < 0) {
        LOG(lfatal, "Impossible to obtain a valid file descriptor");
        exit(1);
    }

    if (bind(pollfd.fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        LOG(lfatal, "Impossible to bind to port: " + std::to_string(get_port()));
        exit(1);
    }

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
        LOG(ltrace, "In the main loop");
        int poll_ret = poll(&pollfd, 1, TIMEOUT_WAIT);
        if (poll_ret > 0) {
            if (pollfd.revents & POLLIN) {
                LOG(ltrace, "Detected POLLIN event");
                ssize_t i = read(pollfd.fd, buf, BUFF_SIZE);
                if (i > 0) {
                    LOG(ltrace, "Data received");
                    auto packet_printer = [] (char* buffer) {
                        LOG(ltrace, "-----------PACKET-----------");
                        LOG(ltrace, buffer);
                        LOG(ltrace, "----------------------------");
                    };
                    ASYNC_TASK(std::bind<void>(packet_printer, buf));
                }
            }
        }
    }
}

void RawSocketUDPConnectionManager::stop() {

}

void RawSocketUDPConnectionManager::send(const char* message,
                                         std::function<void(const char*, int, std::size_t)>& callback) {

}

void RawSocketUDPConnectionManager::send(const char* message) {

}
}
