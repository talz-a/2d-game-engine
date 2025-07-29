#pragma once

#include <unordered_map>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

class AssetStore {
    private:
        std::unordered_map<std::string, SDL_Texture*> textures;

    public:
        AssetStore();
        ~AssetStore();

        void ClearAssets();
        void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
        [[nodiscard]] SDL_Texture* GetTexture(const std::string& assetId);
};
