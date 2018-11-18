//
// Created by zanna on 17/09/18.
//

#include "printerhandler.h"
namespace connection{
namespace handler {

PrinterHandler::PrinterHandler() : AbsHandler(utils::HandlerFields::DEFAULT_CONFIG_PATH) {}

msgptr PrinterHandler::handler_request(msgptr message, std::size_t size) {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    double seconds = te.tv_sec + (te.tv_usec * 0.000001);
    auto file = fopen("test.txt", "a");
    fprintf(file, "%f\n", seconds);
    fclose(file);
    return message;
}

} // namespace handler
} // namespace connection
