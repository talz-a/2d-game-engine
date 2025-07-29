#pragma once

#include "../ECS/ECS.hpp"

#include <SDL.h>

inline constexpr int FPS = 60;
inline constexpr int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        bool isRunning;
        int millisecsPreviousFrame;
        SDL_Window* window;
        SDL_Renderer* renderer;
        std::unique_ptr<Registry> registry;

    public:
        Game();
        ~Game();
        void Initialize();
        void Run();
        void Setup();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();

        int windowWidth;
        int windowHeight;
};
