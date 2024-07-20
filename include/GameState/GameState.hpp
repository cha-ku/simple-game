#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "../ECS/ECS.hpp"
#include "../Logger/Logger.hpp"//TODO: Replace with spdlog at some point
#include "AssetManager.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

const auto FPS = 60;
constexpr auto MILLISECS_PER_FRAME = 1000 / FPS;

class GameState {
private:
  bool isRunning{false};
  SDL_Window* window;
  SDL_Renderer* renderer;
  uint64_t milliSecsPrevFrame = 0;
  std::unique_ptr<Registry> registry{std::make_unique<Registry>()};
  std::unique_ptr<AssetManager> assetStore{std::make_unique<AssetManager>()};

public:
  GameState() = default;
  ~GameState() = default;
  GameState(GameState&)=delete;
  GameState& operator=(GameState&)=delete;
  GameState(GameState&&)=delete;
  GameState& operator=(GameState&&)=delete;
  void Initialize();
  void ProcessInput();
  void Setup();
  void Update();
  void Render();
  void Run();
  void Destroy();
  uint16_t windowWidth = 1024;
  uint16_t windowHeight = 768;
};
#endif
