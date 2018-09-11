#include "log.h"

namespace utils {
Log* Log::inst = nullptr;
Log::Level Log::log_level = Log::Level::trace;
std::string Log::prefix = "";

std::string Log::level_lookup(Level to_lookup) const {
    std::string res;
    switch (to_lookup) {
    case Level::trace : res = "trace"; break;
    case Level::debug : res = "debug"; break;
    case Level::info : res = "info"; break;
    case Level::warning : res = "warning"; break;
    case Level::fatal : res = "fatal"; break;
    }

    return res;
}

std::string Log::format_log(
        const std::string & level,
        const std::string & message) const {

    std::string complete_prefix = prefix != "" ? prefix + " - " : "";
    return complete_prefix + "[" + level + "] - " + message;
}

void Log::log(Level level, const std::string & to_print) const {
    // TODO: add time!
    if (level >= log_level) {
        if (level == Log::Level::fatal) {
            std::cerr << format_log(level_lookup(level), to_print) << std::endl;
        } else {
            std::cout << format_log(level_lookup(level), to_print) << std::endl;
        }
    }
}

void Log::set_log_level(Level new_log_level) const {
    log_level = new_log_level;
}

void Log::set_prefix(const std::string & new_prefix) const {
    prefix = new_prefix;
}

Log::Log() noexcept {
}

Log::~Log() {
}

Log* Log::instance() {
    if (inst == nullptr) {
        inst = new Log();
    }
    return inst;
}
}
