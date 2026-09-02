#pragma once

namespace eng::log {

enum class Level { Trace, Info, Warn, Error };

void Write(Level level, const char* file, int line, const char* fmt, ...);

} // namespace eng::log

#define ENGINE_LOG_TRACE(...) ::eng::log::Write(::eng::log::Level::Trace, __FILE__, __LINE__, __VA_ARGS__)
#define ENGINE_LOG_INFO(...)  ::eng::log::Write(::eng::log::Level::Info,  __FILE__, __LINE__, __VA_ARGS__)
#define ENGINE_LOG_WARN(...)  ::eng::log::Write(::eng::log::Level::Warn,  __FILE__, __LINE__, __VA_ARGS__)
#define ENGINE_LOG_ERROR(...) ::eng::log::Write(::eng::log::Level::Error, __FILE__, __LINE__, __VA_ARGS__)