#pragma once

#include <glm/glm.hpp>

struct BoxColliderComponent {
    int width;
    int height;
    glm::vec2 offset;
    bool currentlyColliding = false;

    BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2{0})
        : width(width),
        height(height),
        offset(offset)
    {}
};
