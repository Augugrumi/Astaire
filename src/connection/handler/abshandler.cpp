//
// Created by zanna on 17/09/18.
//

#include "abshandler.h"

namespace connection{
    namespace handler {
        AbsHandler::AbsHandler(const std::string& config_path) : config(new utils::JsonUtils::JsonWrapper(config_path)) {}

        AbsHandler::~AbsHandler() {
            delete this->config;
        }
    } // namespace handler
} // namespace connection