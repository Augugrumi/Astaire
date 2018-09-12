#include "udpconnectionmanager.h"

namespace connection {
UDPConnectionManager::UDPConnectionManager(
        boost::asio::io_service & new_service,
        unsigned short int port)
    : socket(std::unique_ptr<bip::udp::socket>(
                 new bip::udp::socket(
                     new_service,
                     bip::udp::endpoint(bip::udp::v4(), port)))),
      service(std::unique_ptr<boost::asio::io_service>(&new_service)) {
}

void UDPConnectionManager::run() {
    LOG(ltrace, "Running the server");

        LOG(ltrace, "Ready for receiving another message");
        socket->async_receive_from(
                    boost::asio::buffer(buffer),
                    endpoint,
                    boost::bind(&UDPConnectionManager::handle_message, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
        LOG(ltrace, "Async set");
        service->run();

}

void UDPConnectionManager::stop() {
    service->stop();
    socket->shutdown(boost::asio::socket_base::shutdown_type::shutdown_both);
}

void UDPConnectionManager::handle_message(
        const boost::system::error_code& error,
        std::size_t buffer_size) const {
    if (!error || error == boost::asio::error::message_size) {
        LOG(ldebug, "Message arrived");
        /*for (unsigned long i = 0; i < buffer_size && i < buffer.size(); i++) {
            LOG(ltrace, std::to_string(buffer.at(i)));
        }*/
        LOG(ltrace, "Buffer size " + std::to_string(buffer.size()));
        LOG(ltrace, "Received size " + std::to_string(buffer_size));

        std::shared_ptr<std::string> message (
                    std::shared_ptr<std::string>(
                        new std::string("Hello world")));

        socket->async_send_to(boost::asio::buffer(*message), endpoint,
                  boost::bind(&UDPConnectionManager::reply_to_message,
                              this,
                              message,
                              boost::asio::placeholders::error,
                              boost::asio::placeholders::bytes_transferred));
    }
    const_cast<UDPConnectionManager*>(this)->run();
}

void UDPConnectionManager::reply_to_message(
        std::shared_ptr<std::string> message,
        const boost::system::error_code& error_code,
        std::size_t buffer_size) const {
    LOG(ltrace, "In reply_to_message");
}
}
