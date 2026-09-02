#include "platform/native_handle.h"
#include "core/log.h"

#include <SDL3/SDL.h>

namespace eng::platform {

NativeHandles Query(SDL_Window* window) {
    NativeHandles out;
    if (!window) return out;

    SDL_PropertiesID props = SDL_GetWindowProperties(window);
    if (props == 0) {
        ENGINE_LOG_ERROR("SDL_GetWindowProperties failed: %s", SDL_GetError());
        return out;
    }

#if defined(SDL_PLATFORM_WIN32)
    out.windowHandle = SDL_GetPointerProperty(
        props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);

#elif defined(SDL_PLATFORM_MACOS)
    // bgfx accepts the NSWindow* directly and builds the CAMetalLayer itself.
    out.windowHandle = SDL_GetPointerProperty(
        props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);

#elif defined(SDL_PLATFORM_LINUX) || defined(SDL_PLATFORM_FREEBSD)
    const char* driver = SDL_GetCurrentVideoDriver();

    if (driver && SDL_strcmp(driver, "wayland") == 0) {
        out.isWayland = true;
        out.displayHandle = SDL_GetPointerProperty(
            props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
        out.windowHandle = SDL_GetPointerProperty(
            props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
    } else {
        out.displayHandle = SDL_GetPointerProperty(
            props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
        const Sint64 x11win = SDL_GetNumberProperty(
            props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        out.windowHandle = reinterpret_cast<void*>(static_cast<uintptr_t>(x11win));
    }
#endif

    if (!out.windowHandle) {
        ENGINE_LOG_ERROR("Could not obtain a native window handle from SDL.");
    }
    return out;
}

} // namespace eng::platform