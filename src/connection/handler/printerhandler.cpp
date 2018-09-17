//
// Created by zanna on 17/09/18.
//

#include "printerhandler.h"
namespace connection{
namespace handler {

PrinterHandler::PrinterHandler() : AbsHandler(utils::JsonUtils::DEFAULT_CONFIG_PATH) {}

void PrinterHandler::handler_request(unsigned char *message, std::size_t size) {
    std::cout << "Message size:\n\t" << size << std::endl;
    std::cout << "Message:\n\t" << message << std::endl;
}

} // namespace handler
} // namespace connection