#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Logger.hpp" //TODO: Replace with spdlog at some point
#include "ECS.hpp"
#include "AssetManager.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

constexpr auto FPS = 60;
constexpr auto MILLISECS_PER_FRAME = 1000 / FPS;

class GameState {
private:
  bool isRunning{false};
  SDL_Window* window{nullptr};
  SDL_Renderer* renderer{nullptr};
  uint64_t milliSecsPrevFrame = 0;
  std::unique_ptr<Registry> registry;
  std::unique_ptr<AssetManager> assetStore;

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
  static constexpr uint16_t windowWidth = 1024;
  static constexpr uint16_t windowHeight = 768;
};
#endif
