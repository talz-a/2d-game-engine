#pragma once

#include <glm/glm.hpp>
#include <SDL.h>

struct ProjectileEmitterComponent {
    glm::vec2 projectileVelcoity;
    int repeatFrequency;
    int projectileDuration;
    int hitPercentDamagae;
    bool isFriendly;
    uint lastEmissionTime;

    ProjectileEmitterComponent(glm::vec2 projectileVelcoity = glm::vec2{0}, int repeatFrequency = 0, int projectileDuration = 10000, int hitPercentDamagae = 10, bool isFriendly = false)
        : projectileVelcoity{projectileVelcoity},
        repeatFrequency{repeatFrequency},
        projectileDuration{projectileDuration},
        hitPercentDamagae{hitPercentDamagae},
        isFriendly{isFriendly},
        lastEmissionTime{SDL_GetTicks()}
    { }
};
