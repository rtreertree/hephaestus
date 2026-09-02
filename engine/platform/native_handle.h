#pragma once

struct SDL_Window;

namespace eng::platform {

// Raw native pointers pulled out of an SDL3 window, in the form bgfx expects.
struct NativeHandles {
    void* windowHandle  = nullptr;  // HWND / NSWindow* / xcb window / wl_surface*
    void* displayHandle = nullptr;  // nullptr / nullptr / Display* / wl_display*
    bool  isWayland     = false;
};

// Returns all-null handles on failure; check windowHandle before use.
NativeHandles Query(SDL_Window* window);

} // namespace eng::platform