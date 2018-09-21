//
// Created by zanna on 17/09/18.
//

#include "printerhandler.h"
namespace connection{
namespace handler {

PrinterHandler::PrinterHandler() : AbsHandler(utils::JsonUtils::DEFAULT_CONFIG_PATH) {}

msgptr PrinterHandler::handler_request(msgptr message, std::size_t size) {
#if HAS_UDP
    std::cout << "Message size:\t" << size << std::endl;
    std::cout << "Message num :\t" << connection::RawSocketUDPConnectionManager::ct << std::endl;
    std::cout << "--------------------" << std::endl;
    return message;
#else
    std::cout << "Message size:\n\t" << size << std::endl;
    std::cout << "Message num :\n\t" << message << std::endl;
    return message;
#endif
}

} // namespace handler
} // namespace connection