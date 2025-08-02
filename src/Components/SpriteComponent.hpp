#pragma once

#include <glm/glm.hpp>
#include <SDL.h>
#include <string>

struct SpriteComponent {
    std::string assetId;
    int width;
    int height;
    int zIndex;
    SDL_Rect srcRect;

    SpriteComponent(std::string assetId = "", int width = 0, int height = 0, int zIndex = 0, int srcRectX = 0, int srcRectY = 0)
        : assetId{assetId},
        width{width},
        height{height},
        zIndex{zIndex},
        srcRect{.x = srcRectX, .y = srcRectY, .w = width, .h = height}
    {}
};
