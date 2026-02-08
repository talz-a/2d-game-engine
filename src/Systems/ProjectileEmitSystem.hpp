#pragma once

#include "../Components/BoxColliderComponent.hpp"
#include "../Components/CameraFollowComponent.hpp"
#include "../Components/ProjectileComponent.hpp"
#include "../Components/ProjectileEmitterComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../ECS/ECS.hpp"
#include "../EventBus/EventBus.hpp"
#include "../Events/KeyPressedEvent.hpp"

#include <SDL.h>

class ProjectileEmitSystem : public System {
public:
    ProjectileEmitSystem() {
        RequireComponent<ProjectileEmitterComponent>();
        RequireComponent<TransformComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        // Subscribe to event of SPACE key being pressed.
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::onKeyPressed);
    }

    void onKeyPressed(KeyPressedEvent& event) {
        if (event.symbol == SDLK_SPACE) {
            for (auto entity : GetSystemEntities()) {
                if (!entity.HasComponent<CameraFollowComponent>()) continue;

                const auto projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
                const auto transform = entity.GetComponent<TransformComponent>();
                const auto rigidBody = entity.GetComponent<RigidBodyComponent>();

                // Start the projectile in the middle position.
                glm::vec2 projectilePosition = transform.position;
                if (entity.HasComponent<SpriteComponent>()) {
                    const auto sprite = entity.GetComponent<SpriteComponent>();
                    projectilePosition.x += (transform.scale.x * sprite.width) / 2.0f;
                    projectilePosition.y += (transform.scale.y * sprite.height) / 2.0f;
                }

                // Check which direction to send the bullet.
                glm::vec2 projectileVelocity = projectileEmitter.projectileVelcoity;
                int directionX = 0;
                int directionY = 0;
                if (rigidBody.velocity.x > 0) directionX = +1;
                if (rigidBody.velocity.x < 0) directionX = -1;
                if (rigidBody.velocity.y > 0) directionY = +1;
                if (rigidBody.velocity.y < 0) directionY = -1;
                projectileVelocity.x = projectileEmitter.projectileVelcoity.x * directionX;
                projectileVelocity.y = projectileEmitter.projectileVelcoity.y * directionY;

                Entity projectile = entity.registry->CreateEntity();
                projectile.AddComponent<TransformComponent>(
                    projectilePosition, glm::vec2{1.0, 1.0}, 0
                );
                projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
                projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                projectile.AddComponent<BoxColliderComponent>(4, 4);
                projectile.AddComponent<ProjectileComponent>(
                    projectileEmitter.isFriendly,
                    projectileEmitter.hitPercentDamagae,
                    projectileEmitter.projectileDuration
                );
            }
        }
    }

    void Update(std::unique_ptr<Registry>& registry) {
        for (const auto entity : GetSystemEntities()) {
            auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
            const auto transform = entity.GetComponent<TransformComponent>();

            // Things that should not be repeated.
            if (projectileEmitter.repeatFrequency == 0) continue;

            // Check if it is time to re-emit a new projectile.
            if (SDL_GetTicks() - projectileEmitter.lastEmissionTime >
                projectileEmitter.repeatFrequency) {
                // Start the projectile in the middle position.
                glm::vec2 projectilePosition = transform.position;
                if (entity.HasComponent<SpriteComponent>()) {
                    const auto sprite = entity.GetComponent<SpriteComponent>();
                    projectilePosition.x += (transform.scale.x * sprite.width) / 2.0f;
                    projectilePosition.y += (transform.scale.y * sprite.height) / 2.0f;
                }

                // Add a new projectile to the registry.
                Entity projectile = registry->CreateEntity();
                projectile.AddComponent<TransformComponent>(
                    projectilePosition, glm::vec2{1.0, 1.0}, 0
                );
                projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelcoity);
                projectile.AddComponent<SpriteComponent>("bullet-image", 4, 4, 4);
                projectile.AddComponent<BoxColliderComponent>(4, 4);
                projectile.AddComponent<ProjectileComponent>(
                    projectileEmitter.isFriendly,
                    projectileEmitter.hitPercentDamagae,
                    projectileEmitter.projectileDuration
                );

                projectileEmitter.lastEmissionTime = SDL_GetTicks();
            }
        }
    }
};
