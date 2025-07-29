#pragma once

#include "../ECS/ECS.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/SpriteComponent.hpp"

#include <format>
#include <SDL.h>

class RenderSystem: public System {
    public:
        RenderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(SDL_Renderer* renderer) {
            for (auto entity : GetSystemEntities()) {
                const auto transform = entity.GetComponent<TransformComponent>();
                const auto sprite = entity.GetComponent<SpriteComponent>();
                SDL_Rect objRect = {
                    .x = static_cast<int>(transform.position.x),
                    .y = static_cast<int>(transform.position.y),
                    .w = sprite.width,
                    .h = sprite.height,
                };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &objRect);
            }
        }
};
