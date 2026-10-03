#include "core/log.h"
#include "core/types.h"

#include <SDL3/SDL_timer.h>

#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace eng::log {
    namespace {
        const char *LevelTag(Level l) {
            switch (l) {
            case Level::Trace:
                return "TRACE";
            case Level::Info:
                return "INFO ";
            case Level::Warn:
                return "WARN ";
            case Level::Error:
                return "ERROR";
            }
            return "?????";
        }

        const char *BaseName(const char *path) {
            const char *slash = std::strrchr(path, '/');
#if defined(_WIN32)
            const char *back = std::strrchr(path, '\\');
            if (back > slash)
                slash = back;
#endif
            return slash ? slash + 1 : path;
        }

    } // namespace

    void Write(Level level, const char *file, int line, const char *fmt, ...) {
        char msg[2048];

        va_list args;
        va_start(args, fmt);
        std::vsnprintf(msg, sizeof(msg), fmt, args);
        va_end(args);

        const u64 ns = SDL_GetTicksNS();
        const unsigned long long sec = ns / 1000000000ull;
        const unsigned ms =  static_cast<unsigned>(ns % 1000000000ull) / 100ull;
        std::FILE *out = (level == Level::Error || level == Level::Warn) ? stderr : stdout;
        std::fprintf(out, "[%s][%3llu.%07u] %s:%d: %s\n", LevelTag(level), static_cast<unsigned long long>(sec), ms, BaseName(file), line, msg);
        std::fflush(out);
    }

} // namespace eng::log