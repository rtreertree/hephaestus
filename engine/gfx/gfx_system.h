#pragma once
#include "core/types.h"
#include "platform/native_handle.h"

namespace eng::gfx {

struct GfxDesc {
    platform::NativeHandles native{};
    u32  width  = 1280;
    u32  height = 720;
    bool vsync  = true;
};

class GfxSystem {
public:
    GfxSystem() = default;
    ~GfxSystem();

    GfxSystem(const GfxSystem&)            = delete;
    GfxSystem& operator=(const GfxSystem&) = delete;

    bool Init(const GfxDesc& desc);
    void Shutdown();

    void Resize(u32 width, u32 height);

    // Clears the backbuffer and sets up view rects for this frame.
    void BeginFrame();
    // Submits everything queued and advances bgfx.
    void EndFrame();

    u32 Width()  const { return m_width; }
    u32 Height() const { return m_height; }

private:
    u32  m_width       = 0;
    u32  m_height      = 0;
    u32  m_resetFlags  = 0;
    bool m_initialized = false;
};

} // namespace eng::gfx
