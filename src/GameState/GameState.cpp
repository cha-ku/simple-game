#include "GameState.hpp"
#include "RigidBodyComponent.hpp"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"
#include "MovementSystem.hpp"
#include "RenderSystem.hpp"

void GameState::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Error("Error Initializing SDL");
        return;
    }

    //SDL_DisplayMode displayMode;
    window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_BORDERLESS);
    if (window == nullptr) {
        Logger::Error("Error creating SDL window");
        return;
    }

    // SDL_RENDERER_ACCELERATED  - manually instruct SDL to try to use accelerated GPU
    // SDL_RENDERER_PRESENTVSYNC - use VSync, i.e. sync frame rate with monitor's refresh rate. Enabling VSync will prevent some screen tearing artifacts
    //                             when we display displaying our objects in our game loop, as it will try to synchronize the rendering of our frame with
    //                             the refresh rate of the monitor.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) {
        Logger::Error("Error creating SDL renderer");
        return;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    isRunning = true;
}

void GameState::Setup() {
  registry->AddSystem<MovementSystem>();
  registry->AddSystem<RenderSystem>();

  assetStore->AddTexture(std::string("tank-right"),std::string("/home/chaku/myworkspace/projects/stabby2d/assets/images/tank-panther-right.png"), renderer);

  // Create truck
  auto truck = registry->CreateEntity();
  truck.AddComponent<TransformComponent>(Position(10.0, 30.0), Scale(1.0, 1.0), Rotation(0.0));
  truck.AddComponent<RigidBodyComponent>(Velocity(10, 20));
  truck.AddComponent<SpriteComponent>(std::string("tank-right"), 32, 32, SDL_Rect(0, 0, 32, 32));
}

void GameState::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
                break;
            default:
              Logger::Error("Received bad event");
              break;
        }
    }
}

void GameState::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  registry->GetSystem<RenderSystem>().Update(renderer, assetStore);
  SDL_RenderPresent(renderer);
}


void GameState::Update() {
  // wait until we reach MILLISECS_PER_FRAME
  // ticks = millisecond in SDL parlance
  // SDL_GetTicks() gets no. of millisecs from when the last time SDL_Init() was called

  // Dumb way(blocks other processes) - wait in this loop until current time > past frame draw time + millisecs per frame
  //while(!SDL_TICKS_PASSED(SDL_GetTicks(), milliSecsPrevFrame + MILLISECS_PER_FRAME));

  auto timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks64() - milliSecsPrevFrame);
  if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
      SDL_Delay(timeToWait);
  }

  constexpr double updateInterval = 1000;
  // Time since last frame in seconds
  auto deltaTime = static_cast<double>((SDL_GetTicks64() - milliSecsPrevFrame)) / updateInterval;
  milliSecsPrevFrame = SDL_GetTicks64();
  registry->Update();
  registry->GetSystem<MovementSystem>().Update(deltaTime);
}

void GameState::Run() {
    Logger::Info("Game starting");
    Setup();
    while(isRunning) {
        ProcessInput();
        Update();
        Render();
    }
    Logger::Info("Game ended");
};

void GameState::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
