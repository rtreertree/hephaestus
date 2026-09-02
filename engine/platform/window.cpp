#include "platform/window.h"
#include "core/log.h"
#include "core/assert.h"

#include <SDL3/SDL.h>

namespace eng::platform {

Window::~Window() { Shutdown(); }

bool Window::Init(const WindowDesc& desc) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        ENGINE_LOG_ERROR("SDL_Init failed: %s", SDL_GetError());
        return false;
    }
    m_ownsSdl = true;

    SDL_WindowFlags flags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
    if (desc.resizable) flags |= SDL_WINDOW_RESIZABLE;

    m_window = SDL_CreateWindow(desc.title,
                                static_cast<int>(desc.width),
                                static_cast<int>(desc.height),
                                flags);
    if (!m_window) {
        ENGINE_LOG_ERROR("SDL_CreateWindow failed: %s", SDL_GetError());
        Shutdown();
        return false;
    }

    int pw = 0, ph = 0;
    SDL_GetWindowSizeInPixels(m_window, &pw, &ph);
    m_width  = static_cast<u32>(pw);
    m_height = static_cast<u32>(ph);

    m_native = Query(m_window);
    if (!m_native.windowHandle) {
        Shutdown();
        return false;
    }

    ENGINE_LOG_INFO("Window created: %ux%u (video driver: %s)",
                    m_width, m_height, SDL_GetCurrentVideoDriver());
    return true;
}

void Window::Shutdown() {
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    if (m_ownsSdl) {
        SDL_Quit();
        m_ownsSdl = false;
    }
}

bool Window::PumpEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_EVENT_QUIT:
                m_running = false;
                break;

            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                if (e.window.windowID == SDL_GetWindowID(m_window)) m_running = false;
                break;

            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
                m_width   = static_cast<u32>(e.window.data1);
                m_height  = static_cast<u32>(e.window.data2);
                m_resized = true;
                break;
            }

            case SDL_EVENT_KEY_DOWN:
                if (e.key.key == SDLK_ESCAPE) m_running = false;
                break;

            default:
                break;
        }
    }
    return m_running;
}

bool Window::ConsumeResize() {
    const bool r = m_resized;
    m_resized = false;
    return r;
}

} // namespace eng::platform