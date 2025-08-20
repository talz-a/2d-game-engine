#pragma once

#include "../ECS/ECS.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/TransformComponent.hpp"

#include <SDL.h>

class RenderColliderSystem: public System {
    public:
        RenderColliderSystem() {
            RequireComponent<BoxColliderComponent>();
            RequireComponent<TransformComponent>();
        }

        void Update(SDL_Renderer* renderer, SDL_Rect camera) {
            for (auto entity : GetSystemEntities()) {
                auto transform = entity.GetComponent<TransformComponent>();
                auto boxColider = entity.GetComponent<BoxColliderComponent>();
                SDL_Rect boxColliderRect = {
                    .x = static_cast<int>(transform.position.x + boxColider.offset.x - camera.x),
                    .y = static_cast<int>(transform.position.y + boxColider.offset.y - camera.y),
                    .w = static_cast<int>(boxColider.width * transform.scale.x),
                    .h = static_cast<int>(boxColider.height * transform.scale.y),
                };
                // FIXME: only works for 2 entites with collision component
                if (boxColider.currentlyColliding) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                }
                SDL_RenderDrawRect(renderer, &boxColliderRect);
            }
        }
};
