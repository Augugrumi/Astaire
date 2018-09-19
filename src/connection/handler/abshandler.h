//
// Created by zanna on 17/09/18.
//

#ifndef ASTAIRE_ABSHANDLER_H
#define ASTAIRE_ABSHANDLER_H

#include <iostream>
#include <memory>
#include <jsonutils.h>

typedef std::shared_ptr<uint8_t> msgptr;

namespace connection{

    namespace handler{

        class AbsHandler {
        protected:
            utils::JsonUtils::JsonWrapper* config;
        public:
            AbsHandler(const std::string& config_path);
            virtual msgptr handler_request(msgptr, std::size_t)  = 0;
            virtual ~AbsHandler();
        };

    } // namespace handler
} // namespace connection

#endif //ASTAIRE_ABSHANDLER_H
