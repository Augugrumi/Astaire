//
// Created by zanna on 17/09/18.
//

#ifndef ASTAIRE_ABSHANDLER_H
#define ASTAIRE_ABSHANDLER_H

#include <iostream>
#include <jsonutils.h>

namespace connection{

    namespace handler{

        class AbsHandler {
        protected:
            utils::JsonUtils::JsonWrapper* config;
        public:
            AbsHandler(const std::string& config_path);
            virtual void handler_request(unsigned char* message, std::size_t size) const =0;
            virtual ~AbsHandler();
        };

    } // namespace handler
} // namespace connection

#endif //ASTAIRE_ABSHANDLER_H
