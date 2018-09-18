//
// Created by zanna on 17/09/18.
//

#include "handlercreator.h"

namespace connection{
namespace handler{

AbsHandler *HandlerCreator::getHandlerByLanguageName(
        const std::string &language,
        const std::string &config_file) {
#if HAS_JNI
    if (boost::iequals("java", language)) {
        LOG(linfo, "Using Java handler");
        return new JavaHandler(config_file);
    }
#endif
    LOG(linfo, "No suitable handler detected, using default one");
    return new PrinterHandler();
}

} // namespace handler
} // namespace connection
