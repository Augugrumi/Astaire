//
// Created by zanna on 17/09/18.
//

#ifndef ASTAIRE_HANDLERCREATOR_H
#define ASTAIRE_HANDLERCREATOR_H

#include <string>
#include <boost/algorithm/string.hpp>

#include "log.h"
#include "handler/abshandler.h"
#include "handler/javahandler.h"
#include "handler/printerhandler.h"

namespace connection {
    namespace handler {

        class HandlerCreator {
        public:
            static AbsHandler* getHandlerByLanguageName(const std::string & language, const std::string & config_file);
        };

    } // namespace handler
} // namespace connection

#endif //ASTAIRE_HANDLERCREATOR_H
