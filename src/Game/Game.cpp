#include "Game.hpp"
#include "../Logger/Logger.hpp"
#include "../ECS/ECS.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Systems/MovementSystem.hpp"
#include "../Systems/RenderSystem.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <memory>

Game::Game() {
    isRunning = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    Logger::Log("Game constructor has been called!");
}

Game::~Game() { Logger::Log("Game destructor has been called!"); }

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Err("Error initializing SDL.");
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = 800;   // displayMode.w;
    windowHeight = 600;  // displayMode.h;
    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0); // Can set SDL_WINDOW_BORDERLESS in last pos
    if (!window) {
        Logger::Err("Error creating SDL Window.");
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        Logger::Err("Error creating SDL renderer.");
        return;
    }

    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    isRunning = true;
}

void Game::ProcessInput() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
                break;
        }
    }
}

void Game::Setup() {
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();

    assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");

    Entity tank = registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2{10.0, 30.0}, glm::vec2{1.0, 1.0}, 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2{40.0, 00.0});
    tank.AddComponent<SpriteComponent>("tank-image", 10, 10);

    Entity truck = registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2{50.0, 100.0}, glm::vec2{1.0, 1.0}, 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2{0.0, 50.0});
    truck.AddComponent<SpriteComponent>("truck-image", 10, 50);
}

void Game::Update() {
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) SDL_Delay(timeToWait);

    float deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
    millisecsPreviousFrame = SDL_GetTicks();

    // process entities that are waiting to be added/deleted
    registry->Update();

    registry->GetSystem<MovementSystem>().Update(deltaTime);
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    registry->GetSystem<RenderSystem>().Update(renderer);

    SDL_RenderPresent(renderer);
}

void Game::Run() {
    Setup();
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
