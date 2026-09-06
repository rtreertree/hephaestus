#include <engine.h>

#include <SDL3/SDL_main.h>

int main(int /*argc*/, char** /*argv*/) {
    SDL_SetMainReady();

    eng::EngineDesc desc;
    desc.title  = "Hephaestus — Engine foundation";
    desc.width  = 480;
    desc.height = 120;
    desc.vsync  = true;

    eng::Engine engine;
    if (!engine.Init(desc)) {
        return 1;
    }

    engine.Run();
    engine.Shutdown();
    return 0;
}