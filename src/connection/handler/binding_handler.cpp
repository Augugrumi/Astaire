#include "binding_handler.h"

namespace connection {
namespace handler {
std::string BindingHandler::get_default_ack() const {
    return "ACK";
}
}
}
