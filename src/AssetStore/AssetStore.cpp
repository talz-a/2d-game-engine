#include "AssetStore.hpp"
#include "../Logger/Logger.hpp"

#include <string>
#include <format>
#include <SDL.h>

AssetStore::AssetStore() {
    Logger::Log("AssetStore constructor called!");
}

AssetStore::~AssetStore() {
    ClearAssets();
    Logger::Log("AssetStore deconstructor called!");
}

void AssetStore::ClearAssets() {
    for (auto& [assetId, texture] : textures) {
        SDL_DestroyTexture(texture);
    }
    textures.clear();
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        Logger::Err(std::format("Failed to load image '{}': {}", filePath, IMG_GetError()));
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        Logger::Err(std::format("Failed to create texture from '{}': {}", filePath, SDL_GetError()));
        return;
    }
    SDL_FreeSurface(surface);
    textures.emplace(assetId, texture);

    Logger::Log(std::format("New texture added to Asset Store with id {}", assetId));
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId) {
    return textures[assetId];
}
