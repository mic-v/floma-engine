#include "Game.h"

#ifdef WINDOW_BUILD
#include <SDL.h>
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "../Logger/Logger.h"
#include "../ECS/ECS.h"

Game::Game() : windowHeight(0), windowWidth(0), is_running(false), millisecsPreviousFrame(0)
{
    Logger::Log("Game created");
}

Game::~Game()
{
    Logger::Log("Game destroyed");
}

void Game::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Logger::Err("Error initializing SDL.");
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    windowHeight = displayMode.h;
    windowWidth = displayMode.w;

    this->m_window = SDL_CreateWindow("Floma Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth, windowHeight,
        0);

    if (!this->m_window) {
        Logger::Err("Error initializing SDL window.");
        return;
    }
    this->m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    if (!this->m_renderer) {
        Logger::Err("Error initializing SDL renderer.");
        return;
    }

    SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);

    this->is_running = true;
}

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup()
{
    playerPosition = glm::vec2(10.0, 20.0);
    playerVelocity = glm::vec2(10.0, 0.0);
    //TODO: Initialize game objects
}

void Game::Run()
{
    Setup();
    while (is_running)
    {
        ProcessInput();
        Update();
        Render();
    }

}

void Game::ProcessInput()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;
            break;
        }
    }
}

void Game::Update()
{
    // If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
        SDL_Delay(timeToWait);

    //The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

    //Store the current frame time
    millisecsPreviousFrame = SDL_GetTicks();

    //std::cout << millisecsPreviousFrame << std::endl;

    //TODO: update game objects
    playerPosition.x += playerVelocity.x * deltaTime;
    //playerPosition.y +=
    playerPosition.y += playerVelocity.y * deltaTime;

   
}

void Game::Render()
{
    SDL_SetRenderDrawColor(m_renderer, 70, 180, 255, 255);
    SDL_RenderClear(m_renderer);

    //TODO: render all game objects

    //Draw a rectangle
    //SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    //SDL_RenderFillRect(m_renderer, &player);

    //Draw a PNG texture
    SDL_Surface* surface = IMG_Load("./assets/images/tank-panther-right.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);


    SDL_Rect dstReact = { (int)playerPosition.x, (int)playerPosition.y, 32, 32 };
    SDL_RenderCopy(m_renderer, texture, NULL, &dstReact);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(m_renderer);

}

void Game::Destroy()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();

}