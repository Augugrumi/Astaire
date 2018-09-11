#ifndef LOG_H
#define LOG_H

#include <string>
#include <iostream>

namespace utils {
class Log {
public:
    enum Level {
        trace,
        debug,
        info,
        warning,
        fatal
    };

    static Log* instance();

    void log(Level, const std::string &) const;
    void set_log_level(Level) const;
    void set_prefix(const std::string &) const;

protected:
    Log() noexcept;
    virtual ~Log();
private:
    static Log* inst;
    static Level log_level;
    static std::string prefix;

    std::string level_lookup(Level) const;
    std::string format_log(const std::string &, const std::string &) const;
};
}
#endif // LOG_H
