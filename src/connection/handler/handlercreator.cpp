//
// Created by zanna on 17/09/18.
//

#include "handlercreator.h"

namespace connection{
namespace handler{

AbsHandler *HandlerCreator::getHandlerByLanguageName(const std::string &language, const std::string &config_file) {
    if (boost::iequals("java", language)) {
        return new JavaHandler(config_file);
    }
    LOG(ldebug, "Returning dummy handler")
    return new PrinterHandler();
}

} // namespace handler
} // namespace connection