#include "engine.h"
#include "core/log.h"

#include <SDL3/SDL_timer.h>
#include <bgfx/bgfx.h>

namespace eng {

bool Engine::Init(const EngineDesc& desc) {
    platform::WindowDesc wd;
    wd.title     = desc.title;
    wd.width     = desc.width;
    wd.height    = desc.height;
    wd.resizable = true;

    if (!m_window.Init(wd)) return false;

    gfx::GfxDesc gd;
    gd.native = m_window.Native();
    gd.width  = m_window.Width();
    gd.height = m_window.Height();
    gd.vsync  = desc.vsync;

    if (!m_gfx.Init(gd)) {
        m_window.Shutdown();
        return false;
    }

    m_initialized = true;
    ENGINE_LOG_INFO("Engine initialized.");
    return true;
}

void Engine::Run() {
    if (!m_initialized) return;

    const u64 freq = SDL_GetPerformanceFrequency();
    u64 last = SDL_GetPerformanceCounter();

    while (m_window.PumpEvents()) {
        const u64 now = SDL_GetPerformanceCounter();
        const f32 dt  = static_cast<f32>(static_cast<f64>(now - last) / static_cast<f64>(freq));
        last = now;

        if (m_window.ConsumeResize()) {
            m_gfx.Resize(m_window.Width(), m_window.Height());
        }
        Frame(dt);
    }
}

void Engine::Frame(f32 deltaSeconds) {
    m_gfx.BeginFrame();

    bgfx::dbgTextClear();
    bgfx::dbgTextPrintf(1, 1, 0x0f, "MyEngine foundation");
    bgfx::dbgTextPrintf(1, 2, 0x0f, "renderer: %s",
                        bgfx::getRendererName(bgfx::getRendererType()));
    bgfx::dbgTextPrintf(1, 3, 0x0f, "%ux%u  dt: %.2f ms",
                        m_gfx.Width(), m_gfx.Height(), deltaSeconds * 1000.0f);
    bgfx::dbgTextPrintf(1, 5, 0x0a, "ESC to quit");

    m_gfx.EndFrame();
}

void Engine::Shutdown() {
    if (!m_initialized) return;
    // Reverse init order.
    m_gfx.Shutdown();
    m_window.Shutdown();
    m_initialized = false;
    ENGINE_LOG_INFO("Engine shut down.");
}

} // namespace eng