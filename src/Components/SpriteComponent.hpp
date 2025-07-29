#pragma once

#include <glm/glm.hpp>

struct SpriteComponent {
    std::string assetId;
    int width;
    int height;

    SpriteComponent(std::string assetId = "", int width = 0, int height = 0): assetId{assetId}, width{width}, height{height} {}
};
