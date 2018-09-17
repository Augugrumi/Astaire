//
// Created by zanna on 17/09/18.
//

#include "handlercreator.h"

namespace connection{
namespace handler{

AbsHandler *HandlerCreator::getHandlerByLanguageName(const std::string &language, const std::string &config_file) {
#if HAS_JNI
    if (boost::iequals("java", language)) {
        return new JavaHandler(config_file);
    }
#endif
    LOG(ldebug, "Returning dummy handler")
    return new PrinterHandler();
}

} // namespace handler
} // namespace connection