#pragma once
#include "core/log.h"

#if defined(_MSC_VER)
    #define ENGINE_DEBUGBREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
    #define ENGINE_DEBUGBREAK() __builtin_trap()
#else
    #include <cstdlib>
    #define ENGINE_DEBUGBREAK() std::abort()
#endif

#if !defined(NDEBUG)
    #define ENGINE_ASSERT(cond, ...)                       \
        do {                                               \
            if (!(cond)) {                                 \
                ENGINE_LOG_ERROR("Assertion failed: " #cond); \
                ENGINE_LOG_ERROR(__VA_ARGS__);             \
                ENGINE_DEBUGBREAK();                       \
            }                                              \
        } while (0)
#else
    #define ENGINE_ASSERT(cond, ...) do { (void)sizeof(cond); } while (0)
#endif