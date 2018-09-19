#include "binding_handler.h"

#include "config.h"
#if HAS_TCP

namespace connection {
namespace handler {
std::string BindingHandler::get_default_ack() const {
    return "ACK";
}
}
}

#endif