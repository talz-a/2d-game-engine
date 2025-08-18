#pragma once

#include "../EventBus/Event.hpp"

#include <SDL.h>

class KeyPressEvent: public Event {
    public:
        SDL_Keycode symbol;
        KeyPressEvent(SDL_Keycode symbol): symbol(symbol) {}
};
