#pragma once

#ifdef WINDOW_BUILD
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#endif

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
public:
    Game();
    ~Game();
    void Initialize();
    void Setup();
    void Run();
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();
private:
    SDL_Renderer* m_renderer;
    SDL_Window* m_window;

    int windowHeight;
    int windowWidth;
    
    bool is_running;

    int millisecsPreviousFrame;
};
