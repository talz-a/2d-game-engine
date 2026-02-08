#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <unordered_map>

class AssetStore {
private:
    std::unordered_map<std::string, SDL_Texture*> textures;

public:
    AssetStore();
    ~AssetStore();

    void ClearAssets();
    void
    AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
    [[nodiscard]] SDL_Texture* GetTexture(const std::string& assetId);
};
