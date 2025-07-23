#pragma once

class MovementSystem: public System {
    public:
        MovementSystem() {
            // TODO: RequireComponent<TransformComponent>();
            // RequireComponent<..>();
        }

        void Update() {
            for (auto entity : GetEntities()) {
                // TODO: Update entity position based on its velocity
            }
        }
};
