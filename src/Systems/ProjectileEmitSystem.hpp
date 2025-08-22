#pragma once

#include "../ECS/ECS.hpp"
#include "../Components/ProjectileEmitterComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/BoxColliderComponent.hpp"

#include <SDL.h>

class ProjectileEmitSystem: public System {
    public:
        ProjectileEmitSystem() {
            RequireComponent<ProjectileEmitterComponent>();
            RequireComponent<TransformComponent>();
        }

        void Update(std::unique_ptr<Registry>& registry) {
            for (const auto entity: GetSystemEntities()) {
                auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
                const auto transform = entity.GetComponent<TransformComponent>();

                // Check if it is time to re-emit a new projectile.
                if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency) {
                    glm::vec2 projectilePosition = transform.position;

                    if (entity.HasComponent<SpriteComponent>()) {
                        const auto sprite = entity.GetComponent<SpriteComponent>();
                        projectilePosition.x += (transform.scale.x * sprite.width) / 2.0f;
                        projectilePosition.y += (transform.scale.y * sprite.height) / 2.0f;
                    }

                    // Add a new projectile to the registry.
                    Entity projectile = registry->CreateEntity();
                    projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2{1.0, 1.0}, 0);
                    projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelcoity);
                    projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                    projectile.AddComponent<BoxColliderComponent>(4, 4);

                    projectileEmitter.lastEmissionTime = SDL_GetTicks();
                }
            }
        }
};
