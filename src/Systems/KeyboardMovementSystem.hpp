#pragma once

#include "../ECS/ECS.hpp"
#include "../EventBus/EventBus.hpp"
#include "../Events/KeyPressedEvent.hpp"

#include <SDL.h>
#include <format>

class KeyboardMovementSystem: public System {
    public:
        KeyboardMovementSystem() { }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<KeyPressEvent>(this, &KeyboardMovementSystem::onKeyPressedEvent);
        }

        void onKeyPressedEvent(KeyPressEvent &event) {
            Logger::Log(std::format("Key Pressed event emitted: [{}]",  SDL_GetKeyName(event.symbol)));
        }
};
