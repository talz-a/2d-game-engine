#pragma once

#include <SDL.h>

struct ProjectileComponent {
    bool isFriendly;
    int hitPercentage;
    int duaration;
    uint startTime;

    ProjectileComponent(bool isFriendly = false, int hitPercentage = 0, int duaration = 0)
        : isFriendly{isFriendly},
        hitPercentage{hitPercentage},
        duaration{duaration},
        startTime{SDL_GetTicks()}
    { }
};
