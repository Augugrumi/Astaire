//
// Created by zanna on 17/09/18.
//

#include <rawsocketudpconnectionmanager.h>
#include "printerhandler.h"
namespace connection{
namespace handler {

PrinterHandler::PrinterHandler() : AbsHandler(utils::JsonUtils::DEFAULT_CONFIG_PATH) {}

msgptr PrinterHandler::handler_request(msgptr message, std::size_t size) {
    std::cout << "Message size:\t" << size << std::endl;
    std::cout << "Message num :\t" << connection::RawSocketUDPConnectionManager::ct << std::endl;
    std::cout << "--------------------" << std::endl;
    return message;
}

} // namespace handler
} // namespace connection