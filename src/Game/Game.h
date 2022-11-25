#pragma once

#ifdef WINDOW_BUILD
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#endif


#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
public:
    Game();
    ~Game();
    void Initialize();
    void LoadLevel(int level);
    void Setup();
    void Run();
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();

private:
    SDL_Renderer* m_renderer;
    SDL_Window* m_window;

    std::unique_ptr<Registry> m_registry;
    std::unique_ptr<AssetStore> m_assetstore;
    //Registry* m_registry;

    int windowHeight;
    int windowWidth;
    
    bool is_running;

    int millisecsPreviousFrame;
};
