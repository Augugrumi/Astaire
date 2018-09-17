//
// Created by zanna on 17/09/18.
//

#include "handlercreator.h"

namespace connection{
namespace handler{

AbsHandler *HandlerCreator::getHandlerByLanguageName(const std::string &language, const std::string &config_file) {
    if (boost::iequals("java", language)) {
        return new handler::JavaHandler(config_file);
    }
    LOG(lfatal, "Program started");
    exit(1);
}

} // namespace handler
} // namespace connection