#pragma once

#include <string>
#include <SDL2/SDL.h>

struct SpriteComponent 
{
    std::string assetID;
    int width;
    int height;
    SDL_Rect srcRect;

    SpriteComponent(std::string assetID = "", int width = 0, int height = 0, int srcRectX = 0, int srcRectY = 0): width(width), height(height)
    {
        this->assetID = assetID;
        this->width = width;
        this->height = height;
        this->srcRect = {srcRectX, srcRectY, width, height};
    }
};