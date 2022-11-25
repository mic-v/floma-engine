#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>

class AssetStore
{
  public:
    AssetStore();
    ~AssetStore();

    void ClearAssets();
    void AddTexture(SDL_Renderer* renderer, const std::string& assetID, const std::string& filePath);
    SDL_Texture* GetTexture(const std::string& assetID);
  private:  
    std::map<std::string, SDL_Texture*> textures;
    // TODO:: create a map for fonts
    // TODO:: create a map for audio
};