#include <bgfx/bgfx.h>
#include "core/log.h"

struct QuietCallback : bgfx::CallbackI {
    void fatal(const char* filePath, uint16_t line, bgfx::Fatal::Enum code, const char* str) override {
        // Keep fatal errors visible, since bgfx can't continue after these
        eng::log::Write(eng::log::Level::Error, filePath, line, "[bgfx FATAL|%s] %s", code, str);
        ENGINE_LOG_ERROR("[%s] bgfx fatal error: %s at %s:%d", code, str, filePath, line);
        abort();
    }

    void traceVargs(const char*, uint16_t, const char*, va_list) override {
        // swallow all trace output
    }

    void profilerBegin(const char*, uint32_t, const char*, uint16_t) override {}
    void profilerBeginLiteral(const char*, uint32_t, const char*, uint16_t) override {}
    void profilerEnd() override {}
    uint32_t cacheReadSize(uint64_t) override { return 0; }
    bool cacheRead(uint64_t, void*, uint32_t) override { return false; }
    void cacheWrite(uint64_t, const void*, uint32_t) override {}
    void screenShot(const char*, uint32_t, uint32_t, uint32_t, bgfx::TextureFormat::Enum, const void*, uint32_t, bool) override {}
    void captureBegin(uint32_t, uint32_t, uint32_t,
                      bgfx::TextureFormat::Enum, bool) override {}
    void captureEnd() override {}
    void captureFrame(const void*, uint32_t) override {}
};