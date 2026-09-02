#include "gfx/gfx_system.h"
#include "gfx/views.h"
#include "core/log.h"

#include <bgfx/bgfx.h>

namespace eng::gfx {

GfxSystem::~GfxSystem() { Shutdown(); }

bool GfxSystem::Init(const GfxDesc& desc) {
    if (!desc.native.windowHandle) {
        ENGINE_LOG_ERROR("GfxSystem::Init called without a native window handle.");
        return false;
    }

    // Forces single-threaded rendering. Must happen BEFORE bgfx::init.
    // Simplifies debugging; drop it later if you want the render thread back.
    bgfx::renderFrame();

    m_width  = desc.width;
    m_height = desc.height;
    m_resetFlags = BGFX_RESET_NONE;
    if (desc.vsync) m_resetFlags |= BGFX_RESET_VSYNC;

    bgfx::Init init;
    init.type              = bgfx::RendererType::Count;  // auto-pick per platform
    init.vendorId          = BGFX_PCI_ID_NONE;
    init.resolution.width  = m_width;
    init.resolution.height = m_height;
    init.resolution.reset  = m_resetFlags;

    init.platformData.nwh = desc.native.windowHandle;
    init.platformData.ndt = desc.native.displayHandle;
#if defined(__linux__) || defined(__FreeBSD__)
    init.platformData.type = desc.native.isWayland
        ? bgfx::NativeWindowHandleType::Wayland
        : bgfx::NativeWindowHandleType::Default;
#endif

    if (!bgfx::init(init)) {
        ENGINE_LOG_ERROR("bgfx::init failed.");
        return false;
    }
    m_initialized = true;

    bgfx::setDebug(BGFX_DEBUG_TEXT);

    bgfx::setViewName(views::kOpaque, "Opaque");
    bgfx::setViewClear(views::kOpaque,
                       BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
                       0x303040ff, 1.0f, 0);

    ENGINE_LOG_INFO("bgfx initialized: renderer=%s %ux%u",
                    bgfx::getRendererName(bgfx::getRendererType()),
                    m_width, m_height);
    return true;
}

void GfxSystem::Shutdown() {
    if (!m_initialized) return;
    bgfx::shutdown();
    m_initialized = false;
    ENGINE_LOG_INFO("bgfx shut down.");
}

void GfxSystem::Resize(u32 width, u32 height) {
    if (!m_initialized) return;
    if (width == 0 || height == 0) return;              // minimized
    if (width == m_width && height == m_height) return;

    m_width  = width;
    m_height = height;
    bgfx::reset(m_width, m_height, m_resetFlags);
    ENGINE_LOG_INFO("Backbuffer reset: %ux%u", m_width, m_height);
}

void GfxSystem::BeginFrame() {
    if (!m_initialized) return;
    bgfx::setViewRect(views::kOpaque, 0, 0,
                      static_cast<uint16_t>(m_width),
                      static_cast<uint16_t>(m_height));
    // Guarantees the view is cleared even with nothing submitted.
    bgfx::touch(views::kOpaque);
}

void GfxSystem::EndFrame() {
    if (!m_initialized) return;
    bgfx::frame();
}

} // namespace eng::gfx