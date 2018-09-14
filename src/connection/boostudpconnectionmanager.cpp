#include "boostudpconnectionmanager.h"

namespace connection {
BoostUDPConnectionManager::BoostUDPConnectionManager(
        boost::asio::io_service & new_service,
        unsigned short int port)
    : socket(std::unique_ptr<bip::udp::socket>(
                 new bip::udp::socket(
                     new_service,
                     bip::udp::endpoint(bip::udp::v4(), port)))),
      service(std::unique_ptr<boost::asio::io_service>(&new_service)) {
}

void BoostUDPConnectionManager::run() {
    LOG(ltrace, "Running the server");

        LOG(ltrace, "Ready for receiving another message");
        socket->async_receive_from(
                    boost::asio::buffer(buffer),
                    endpoint,
                    boost::bind(&BoostUDPConnectionManager::handle_message, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
        LOG(ltrace, "Async set");
        service->run();
}

void BoostUDPConnectionManager::stop() {
    service->stop();
    socket->shutdown(boost::asio::socket_base::shutdown_type::shutdown_both);
}

void BoostUDPConnectionManager::send(
        const char* message,
        std::function<void(const char*, int, std::size_t)> & callback) {

    auto converted_errno = [&callback] (
            const char* message,
            boost::system::error_code err, std::size_t buff_size) {
        callback(message, err.value(), buff_size);
    };

    socket->async_send_to(boost::asio::buffer(message, strlen(message)),
                          endpoint,
                          boost::bind<void>(
                              converted_errno,
                              message,
                              boost::asio::placeholders::error,
                              boost::asio::placeholders::bytes_transferred));
}

void BoostUDPConnectionManager::send(const char* message) {
    socket->send_to(boost::asio::buffer(message, strlen(message)), endpoint);
}

void BoostUDPConnectionManager::handle_message(
        const boost::system::error_code& error,
        std::size_t buffer_size) {
    if (!error || error == boost::asio::error::message_size) {
        LOG(ldebug, "Message arrived");

        auto printer = [] (
                boost::array<char, 65536> buffer, std::size_t buffer_size) {
            LOG(ltrace, "Buffer size " + std::to_string(buffer.size()));
            LOG(ltrace, "Received size " + std::to_string(buffer_size));

            std::string tmp;
            LOG(ltrace, "-------------PACKET CONTENT------------");
            for (std::size_t i = 0; i < buffer_size && i < buffer.size(); i++) {
                tmp.push_back(buffer.at(i));
            }
            LOG(ltrace, tmp);
            LOG(ltrace, "---------------------------------------");
        };
        ASYNC_TASK(std::bind<void>(printer, buffer, buffer_size));


        std::shared_ptr<std::string> message (
                    std::shared_ptr<std::string>(
                        new std::string("Hello world")));

        std::function<void(const char*, int, std::size_t)> cb_sender = [] (
                const char* message, int err, std::size_t buffer_size) {
            LOG(ldebug, "Inside the callback lambda - send");
        };

        send(message->c_str(), cb_sender);
    }
    run();
}

}
