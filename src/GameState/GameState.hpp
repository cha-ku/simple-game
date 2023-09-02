#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "../ECS/ECS.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "../Logger/Logger.hpp" //TODO: Replace with spdlog at some point

const auto FPS = 60;
constexpr auto MILLISECS_PER_FRAME = 1000 / FPS;

class GameState {
private:
  bool isRunning{false};
  SDL_Window* window;
  SDL_Renderer* renderer;
  uint64_t milliSecsPrevFrame = 0;
  glm::vec2 playerPos;
  glm::vec2 playerVel;
  std::unique_ptr<Registry> registry{std::make_unique<Registry>()};

public:
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
