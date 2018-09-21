//
// Created by zanna on 17/09/18.
//

#ifndef ASTAIRE_PRINTERHANDLER_H
#define ASTAIRE_PRINTERHANDLER_H

#include <iostream>
#include "rawsocketudpconnectionmanager.h"
#include "abshandler.h"

namespace connection{
    namespace handler {
        class PrinterHandler : public AbsHandler {
        public:
            PrinterHandler();
            virtual msgptr handler_request(msgptr message, std::size_t size) override;
        };
    } // namespace handler
} // namespace connection

#endif //ASTAIRE_PRINTERHANDLER_H
