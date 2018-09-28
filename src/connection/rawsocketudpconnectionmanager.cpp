#include "rawsocketudpconnectionmanager.h"

#include "config.h"
#if HAS_UDP

namespace connection {

std::atomic_int_fast64_t RawSocketUDPConnectionManager::ct(0);

RawSocketUDPConnectionManager::RawSocketUDPConnectionManager(
        uint32_t to_listen,
        unsigned short int port,
        const std::string & to_send,
        unsigned short int port_to_send,
        std::shared_ptr<handler::AbsHandler> abshandler)
    : UDPConnectionManager(port),
      forward_address(to_send),
      forward_port(port_to_send),
      handler(abshandler),
      run_flag(true) {

    buf = new char[BUFFER_SIZE];

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(to_listen);
    addr.sin_port = htons(get_port());
}

RawSocketUDPConnectionManager::~RawSocketUDPConnectionManager() {
    delete buf;
}

void RawSocketUDPConnectionManager::run() {

    run_flag = true;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    // Setting events for incoming data
    pollfd.fd = fd;
    pollfd.events = POLLIN;
    pollfd.revents = 0;



    if (pollfd.fd < 0) {
        LOG(lfatal, "Impossible to obtain a valid file descriptor");
        exit(1);
    }

    if (bind(pollfd.fd,
             reinterpret_cast<struct sockaddr*>(&addr),
             sizeof(addr)) < 0) {
        LOG(lfatal, "Faliure binding to port: " + std::to_string(get_port()));
        exit(1);
    }

    // Main loop
    while (run_flag) {
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

        pollfd.fd = fd;
        pollfd.events = POLLIN;
        pollfd.revents = 0;

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
                            reinterpret_cast<struct sockaddr*>(&client),
                            &clientlen);

                if (i > 0) {
                    LOG(ltrace, "Data received from recvfrom()");
                    auto packet_printer = [i, this] (msgptr buffer) {

                        /* The packet has arrived and it needs to be processed.
                         * An incoming packet is composed as follows:
                         * +--------------------------------------------------+
                         * + UDP standard headers                             +
                         * +--------------------------------------------------+
                         * + Astaire metadata (delimited with a flag defined  +
                         * + in METADATA_FLAG). Astaire metadata are composed +
                         * + as follows:                                      +
                         * + - SFC id                                         +
                         * + - current SF id                                  +
                         * +--------------------------------------------------+
                         * + The real data payload, to pass to the handler    +
                         * +--------------------------------------------------+
                         *
                         * Computation follows these steps:
                         * - Astaire metadata gets extracted and the next hop
                         *   gets identified
                         * - The data payload (without astaire metadata) gets
                         *   processed by the handler
                         * - The returning data + the astaire metadata - the
                         *   next address hop get forwarded to the next hop.
                         */

                        utils::sfc_header::SFCFixedLengthHeader header(buffer);

                        // TODO Now we need to make a request to obtain the next
                        // hop address


                        // Calling the handler
                        msgptr payload;
                        sfcu::SFCUtilities::retrieve_payload(buffer.get(),
                                static_cast<size_t>(i -
                                sfcu::SFCUtilities::HEADER_SIZE),
                                payload.get());

                        buffer = handler->
                            handler_request(payload,
                                    static_cast<size_t>(i -
                                    sfcu::SFCUtilities::HEADER_SIZE));

                        LOG(ltrace, "Packet count: " + std::to_string(ct));
                        ct++;


                        // Recomposing the payload
                        if (header.get_ttl() > 0) {
                            header.set_ttl(header.get_ttl() - 1);

                            // Forwarding the data
                            uint8_t* new_pkt;
                            sfcu::SFCUtilities::prepend_header(buffer.get(),
                                    i - sfcu::SFCUtilities::HEADER_SIZE,
                                    header.get_header(), new_pkt);
                            send(reinterpret_cast<char*>(new_pkt),
                                 // FIXME the handle could change the packet size!
                                 static_cast<size_t>(i),
                                 // FIXME change with sender address
                                 forward_address.c_str(),
                                 forward_port); // FIXME change with sender port
                        }
                    };

                    msgptr cloned_buffer = msgptr(
                            reinterpret_cast<uint8_t*>(buf));
                    buf = new char[BUFFER_SIZE];

                    std::string ack = "ACK";
                    send(pollfd.fd, ack.c_str(), ack.size(), &client);
                    ASYNC_TASK(std::bind<void>(packet_printer, cloned_buffer));
                } else if (errno != 0) {
                    LOG(lfatal, "Errno: " + std::to_string(errno));
                    LOG(lfatal, strerror(errno));
                    exit(1);
                }
            }
        } else {
            LOG(lfatal, "Error fetching data in poll()");
        }
    }
}

void RawSocketUDPConnectionManager::stop() {
    run_flag = false;
}

void RawSocketUDPConnectionManager::send(
        int fd,
        const char* message,
        size_t mlen,
        sockaddr_in* dest,
        std::function<void(ssize_t)>& cb) const {

    auto async_send = [this] (
            int fd,
            const char* message,
            size_t mlen,
            sockaddr_in* dest,
            std::function <void(ssize_t)>& cb) {
        ssize_t res = send(fd, message, mlen, dest);
        cb(res);
    };

    ASYNC_TASK(std::bind<void>(async_send, fd, message, mlen, dest, cb));
}

ssize_t RawSocketUDPConnectionManager::send(
        int fd,
        const char* message,
        size_t mlen,
        sockaddr_in* dest) const {
    return sendto(
                fd,
                message,
                mlen,
                0,
                reinterpret_cast<struct sockaddr*>(dest),
                sizeof(*dest));
}

ssize_t RawSocketUDPConnectionManager::send(
        const char* message,
        size_t mlen,
        const char* address,
        unsigned short int port) const {

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
        if (sfd > 0 && connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            res = send(
                        sfd,
                        message,
                        mlen,
                        reinterpret_cast<struct sockaddr_in*>(rp->ai_addr));
            if (res > 0) {
                send_flag = false;
            }
        }
        close(sfd);
    }

    freeaddrinfo(result);
    return res;
}

ssize_t RawSocketUDPConnectionManager::sound_send(
        int fd,
        const char* message,
        size_t mlen,
        sockaddr_in* dest,
        short retr) {

    ssize_t result;

    while ((result = send(fd, message, mlen, dest)) < 0 && retr >= 0) {
        retr--;
    }

    if (retr < 0) {
        LOG(lwarn, "Failure when sending packet:\n" + std::to_string(*message));
        return -1;
    }

    return result;
}

void RawSocketUDPConnectionManager::counter_printer(int i) {
    // TODO Switch the signal
    LOG(linfo, "Number of packets: " + std::to_string(ct));
    exit(0);
}
}
#endif
