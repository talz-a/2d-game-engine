#pragma once

#include "../Components/ProjectileComponent.hpp"
#include "../ECS/ECS.hpp"

#include <SDL.h>

class ProjectileLifecycleSystem : public System {
public:
    ProjectileLifecycleSystem() { RequireComponent<ProjectileComponent>(); }

    void Update() {
        for (auto entity : GetSystemEntities()) {
            auto projectile = entity.GetComponent<ProjectileComponent>();
            if (SDL_GetTicks() - projectile.startTime > projectile.duaration) entity.Kill();
        }
    }
};
