#pragma once

#include "../Components/BoxColliderComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../ECS/ECS.hpp"
#include "../EventBus/EventBus.hpp"
#include "../Events/CollisionEvent.hpp"

#include <format>

class CollisionSystem : public System {
public:
    CollisionSystem() {
        RequireComponent<BoxColliderComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(std::unique_ptr<EventBus>& eventBus) {
        std::vector<Entity> entities = GetSystemEntities();
        for (auto i = entities.begin(); i != entities.end(); i++) {
            Entity a = *i;
            auto aTransform = a.GetComponent<TransformComponent>();
            auto& aCollider = a.GetComponent<BoxColliderComponent>();

            for (auto j = i; j != entities.end(); j++) {
                Entity b = *j;

                if (a == b) continue;

                auto bTransform = b.GetComponent<TransformComponent>();
                auto& bCollider = b.GetComponent<BoxColliderComponent>();

                bool collisionHappened = CheckAABBCollision(
                    aTransform.position.x + aCollider.offset.x,
                    aTransform.position.y + aCollider.offset.y,
                    aCollider.width,
                    aCollider.height,
                    bTransform.position.x + bCollider.offset.x,
                    bTransform.position.y + bCollider.offset.y,
                    bCollider.width,
                    bCollider.height
                );
                if (collisionHappened) {
                    Logger::Log(
                        std::format("Entity {} is colliding with Entity {}", a.GetId(), b.GetId())
                    );

                    // FIXME: only works for 2 entites with collision component
                    aCollider.currentlyColliding = true;
                    bCollider.currentlyColliding = true;

                    eventBus->EmitEvent<CollisionEvent>(a, b);
                } else {
                    aCollider.currentlyColliding = false;
                    bCollider.currentlyColliding = false;
                }
            }
        }
    }

    bool CheckAABBCollision(
        double aX,
        double aY,
        double aW,
        double aH,
        double bX,
        double bY,
        double bW,
        double bH
    ) {
        return aX < bX + bW && aX + aW > bX && aY < bY + bH && aY + aH > bY;
    }
};
