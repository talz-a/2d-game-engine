#include "Game.hpp"
#include "../Logger/Logger.hpp"
#include "../ECS/ECS.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/AnimationComponent.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/KeyboardControlledComponent.hpp"
#include "../Components/CameraFollowComponent.hpp"
#include "../Components/ProjectileEmitterComponent.hpp"
#include "../Systems/MovementSystem.hpp"
#include "../Systems/RenderSystem.hpp"
#include "../Systems/CollisionSystem.hpp"
#include "../Systems/AnimationSystem.hpp"
#include "../Systems/RenderColliderSystem.hpp"
#include "../Systems/DamageSystem.hpp"
#include "../Systems/KeyboardMovementSystem.hpp"
#include "../Systems/CameraMovementSystem.hpp"
#include "../Systems/ProjectileEmitSystem.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <memory>
#include <fstream>
#include <string>

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;

Game::Game() {
    isRunning = false;
    isDebug = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    eventBus = std::make_unique<EventBus>();
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

    // windowWidth = displayMode.w; // For fullscreen set = displayMode.w;
    // windowHeight = displayMode.h; // For fullscreen set = displayMode.h;

    windowWidth = 800; // For fullscreen set = displayMode.w;
    windowHeight = 600; // For fullscreen set = displayMode.h;
    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0); // Can set SDL_WINDOW_BORDERLESS in last position.
    if (!window) {
        Logger::Err("Error creating SDL Window.");
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        Logger::Err("Error creating SDL renderer.");
        return;
    }

    // Initialize the camera view with the entire screen.
    camera.y = 0;
    camera.x = 0;
    camera.w = windowWidth;
    camera.h = windowHeight;

    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    isRunning = true;
}


void Game::LoadLevel(int level) {
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<KeyboardMovementSystem>();
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<ProjectileEmitSystem>();

    assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
    assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
    assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
    assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
    assetStore->AddTexture(renderer, "bullet-image", "./assets/images/bullet.png");

    // Load the tilemap.
    int tileSize = 32;
    double tileScale = 1.0;
    int mapNumCols = 25;
    int mapNumRows = 20;
    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/jungle.map");
    for (int y = 0; y < mapNumRows; y++) {
        for (int x = 0; x < mapNumCols; x++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();
            Entity tile = registry->CreateEntity();
            tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
            tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
        }
    }
    mapFile.close();
    mapWidth = mapNumCols * tileSize * tileScale;
    mapHeight = mapNumRows * tileSize * tileScale;

    Entity chopper = registry->CreateEntity();
    chopper.AddComponent<TransformComponent>(glm::vec2{10.0, 100.0}, glm::vec2{1.0, 1.0}, 0.0);
    chopper.AddComponent<RigidBodyComponent>(glm::vec2{0.0, 0.0});
    chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
    chopper.AddComponent<AnimationComponent>(2, 15, true);
    chopper.AddComponent<KeyboardControlledComponent>(glm::vec2{0, -80}, glm::vec2{80, 0}, glm::vec2{0, 80}, glm::vec2{-80, 0});
    chopper.AddComponent<CameraFollowComponent>();

    Entity radar = registry->CreateEntity();
    radar.AddComponent<TransformComponent>(glm::vec2{windowWidth - 74, 10.0}, glm::vec2{1.0, 1.0}, 0.0);
    radar.AddComponent<RigidBodyComponent>(glm::vec2{0.0, 0.0});
    radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 2, true);
    radar.AddComponent<AnimationComponent>(8, 5, true);

    Entity tank = registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2{500.0, 10.0}, glm::vec2{1.0, 1.0}, 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2{0.0, 0.0});
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 2);
    tank.AddComponent<BoxColliderComponent>(32, 32);
    tank.AddComponent<ProjectileEmitterComponent>(glm::vec2{100.0 , 0.0}, 5000, 10000, 0, false);

    Entity truck = registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2{10.0, 10.0}, glm::vec2{1.0, 1.0}, 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2{0.0, 0.0});
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 1);
    truck.AddComponent<BoxColliderComponent>(32, 32);
    truck.AddComponent<ProjectileEmitterComponent>(glm::vec2{0.0 , 100.0}, 2000, 10000, 0, false);
}

void Game::Setup() {
    LoadLevel(1);
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
                if (sdlEvent.key.keysym.sym == SDLK_d) isDebug = !isDebug;
                eventBus->EmitEvent<KeyPressEvent>(sdlEvent.key.keysym.sym);
                break;
        }
    }
}

void Game::Update() {
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) SDL_Delay(timeToWait);

    float deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
    millisecsPreviousFrame = SDL_GetTicks();

    // Reset all event handlers for the current frame.
    eventBus->Reset();

    // Perform the subscription of the events for all systems.
    registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<KeyboardMovementSystem>().SubscribeToEvents(eventBus);

    // Process entities that are waiting to be added/deleted.
    registry->Update();

    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventBus);
    registry->GetSystem<CameraMovementSystem>().Update(camera);
    registry->GetSystem<ProjectileEmitSystem>().Update(registry);
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    registry->GetSystem<RenderSystem>().Update(renderer, camera, assetStore);

    if (isDebug) registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);

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
