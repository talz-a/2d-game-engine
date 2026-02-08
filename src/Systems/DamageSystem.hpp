#pragma once

#include "../Components/BoxColliderComponent.hpp"
#include "../ECS/ECS.hpp"
#include "../EventBus/EventBus.hpp"
#include "../Events/CollisionEvent.hpp"

#include <format>

class DamageSystem : public System {
public:
    DamageSystem() { RequireComponent<BoxColliderComponent>(); }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
    }

    void onCollision(CollisionEvent& event) {
        Logger::Log(
            std::format(
                "The Damage system recieved an event collision between entities {} and {}",
                event.a.GetId(),
                event.b.GetId()
            )
        );

        // event.a.Kill();
        // event.b.Kill();
    }

    void Update() {}
};
