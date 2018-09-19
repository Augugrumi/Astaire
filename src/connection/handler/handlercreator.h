//
// Created by zanna on 17/09/18.
//

#ifndef ASTAIRE_HANDLERCREATOR_H
#define ASTAIRE_HANDLERCREATOR_H

#include <string>
#include <boost/algorithm/string.hpp>

#include "log.h"
#include "handler/abshandler.h"
#include "handler/printerhandler.h"

#include "config.h"
#if HAS_JNI
#include "handler/javahandler.h"
#endif

namespace connection {
    namespace handler {

        class HandlerCreator {
        public:
            static AbsHandler* getHandlerByLanguageName(
                    const std::string &,
                    const std::string &);
        };

    } // namespace handler
} // namespace connection

#endif //ASTAIRE_HANDLERCREATOR_H
