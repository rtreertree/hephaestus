#include "core/log.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace eng::log {
namespace {

const char* LevelTag(Level l) {
    switch (l) {
        case Level::Trace: return "TRACE";
        case Level::Info:  return "INFO ";
        case Level::Warn:  return "WARN ";
        case Level::Error: return "ERROR";
    }
    return "?????";
}

const char* BaseName(const char* path) {
    const char* slash = std::strrchr(path, '/');
#if defined(_WIN32)
    const char* back = std::strrchr(path, '\\');
    if (back > slash) slash = back;
#endif
    return slash ? slash + 1 : path;
}

} // namespace

void Write(Level level, const char* file, int line, const char* fmt, ...) {
    char msg[2048];

    va_list args;
    va_start(args, fmt);
    std::vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);

    std::FILE* out = (level == Level::Error || level == Level::Warn) ? stderr : stdout;
    std::fprintf(out, "[%s] %s:%d: %s\n", LevelTag(level), BaseName(file), line, msg);
    std::fflush(out);
}

} // namespace eng::log