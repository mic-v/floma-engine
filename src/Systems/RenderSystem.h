#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL.h>

class RenderSystem: public System
{
public:
    RenderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }
    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetstore)
    {
        for(auto entity: GetSystemEntities()) 
        {
            const auto transform = entity.GetComponent<TransformComponent>();
            const auto sprite = entity.GetComponent<SpriteComponent>();

            SDL_Rect srcRect = sprite.srcRect;

            SDL_Rect dstRect = {
                static_cast<int>(transform.position.x),
                static_cast<int>(transform.position.y),
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y)
            };

            //SDL_RenderCopy(renderer, assetstore->GetTexture(sprite.assetID))
            SDL_RenderCopyEx(renderer, 
            assetstore->GetTexture(sprite.assetID),
            &srcRect,
            &dstRect,
            transform.rotation,
            NULL,
            SDL_FLIP_NONE
            );
        }
    }
private:
};