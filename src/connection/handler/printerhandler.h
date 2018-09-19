//
// Created by zanna on 17/09/18.
//

#ifndef ASTAIRE_PRINTERHANDLER_H
#define ASTAIRE_PRINTERHANDLER_H

#include <iostream>

#include "abshandler.h"

namespace connection{
    namespace handler {
        class PrinterHandler : public AbsHandler {
        public:
            PrinterHandler();
            void handler_request(unsigned char *message, std::size_t size) override;
        };
    } // namespace handler
} // namespace connection

#endif //ASTAIRE_PRINTERHANDLER_H
