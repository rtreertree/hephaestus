#pragma once
#include "core/types.h"
#include "platform/window.h"
#include "gfx/gfx_system.h"
#include "gfx/cube_renderer.h"

namespace eng {

struct EngineDesc {
    const char* title  = "MyEngine";
    u32         width  = 1280;
    u32         height = 720;
    bool        vsync  = true;
};

class Engine {
public:
    bool Init(const EngineDesc& desc);
    void Run();
    void Shutdown();

private:
    void Frame(f32 deltaSeconds);

    platform::Window m_window;
    gfx::GfxSystem   m_gfx;
    gfx::CubeRenderer m_cube;
    bool m_initialized = false;
};

} // namespace eng
