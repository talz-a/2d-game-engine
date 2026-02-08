#pragma once

#include <SDL.h>
#include <glm/glm.hpp>
#include <string>

struct SpriteComponent {
    std::string assetId;
    int width;
    int height;
    int zIndex;
    bool isFixed;
    SDL_Rect srcRect;

    SpriteComponent(
        std::string assetId = "",
        int width = 0,
        int height = 0,
        int zIndex = 0,
        bool isFixed = false,
        int srcRectX = 0,
        int srcRectY = 0
    )
        : assetId{assetId},
          width{width},
          height{height},
          zIndex{zIndex},
          isFixed{isFixed},
          srcRect{.x = srcRectX, .y = srcRectY, .w = width, .h = height} {}
};
