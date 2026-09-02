#pragma once
#include "core/types.h"
#include "platform/native_handle.h"

struct SDL_Window;

namespace eng::platform {

struct WindowDesc {
    const char* title  = "MyEngine";
    u32         width  = 1280;
    u32         height = 720;
    bool        resizable = true;
};

class Window {
public:
    Window() = default;
    ~Window();

    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

    bool Init(const WindowDesc& desc);
    void Shutdown();

    // Drains the SDL event queue. Returns false when the app should quit.
    bool PumpEvents();

    // True for exactly one frame after a size change; caller resets via ConsumeResize().
    bool ConsumeResize();

    NativeHandles Native() const { return m_native; }
    SDL_Window*   Raw()    const { return m_window; }
    u32 Width()  const { return m_width; }
    u32 Height() const { return m_height; }

private:
    SDL_Window*   m_window   = nullptr;
    NativeHandles m_native{};
    u32  m_width    = 0;
    u32  m_height   = 0;
    bool m_resized  = false;
    bool m_running  = true;
    bool m_ownsSdl  = false;
};

} // namespace eng::platform