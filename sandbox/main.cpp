#include <engine.h>

#include <SDL3/SDL_main.h>

int main(int /*argc*/, char** /*argv*/) {
    SDL_SetMainReady();

    eng::EngineDesc desc;
    desc.title  = "MyEngine — Sandbox";
    desc.width  = 1280;
    desc.height = 720;
    desc.vsync  = true;

    eng::Engine engine;
    if (!engine.Init(desc)) {
        return 1;
    }

    engine.Run();
    engine.Shutdown();
    return 0;
}