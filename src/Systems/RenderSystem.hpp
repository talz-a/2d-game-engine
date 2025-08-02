#pragma once

#include "../ECS/ECS.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../AssetStore/AssetStore.hpp"

#include <SDL.h>
#include <algorithm>

class RenderSystem: public System {
    public:
        RenderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
            // TODO: sort all the entities of our system by z-index;
            std::vector<Entity> systemEntites = GetSystemEntities();
            std::ranges::sort(systemEntites, std::less<>(), [](const Entity& entity) {
                return entity.GetComponent<SpriteComponent>().zIndex;
            });

            for (auto entity : systemEntites) {
                const auto transform = entity.GetComponent<TransformComponent>();
                const auto sprite = entity.GetComponent<SpriteComponent>();

                SDL_Rect srcRect = sprite.srcRect;
                SDL_Rect dstRect = {
                    .x = static_cast<int>(transform.position.x),
                    .y = static_cast<int>(transform.position.y),
                    .w = static_cast<int>(sprite.width * transform.scale.x),
                    .h = static_cast<int>(sprite.height * transform.scale.y),
                };

                SDL_RenderCopyEx(
                        renderer,
                        assetStore->GetTexture(sprite.assetId),
                        &srcRect,
                        &dstRect,
                        transform.rotation,
                        NULL,
                        SDL_FLIP_NONE
                );
            }
        }
};
