//
// Created by chaku on 15/09/23.
//

#ifndef STABBY2D_RENDERSYSTEM_HPP
#define STABBY2D_RENDERSYSTEM_HPP

#include "../Components/SpriteComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../ECS/ECS.hpp"
#include <SDL2/SDL.h>

class RenderSystem : public System {
public:
  RenderSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<SpriteComponent>();
  }

  void Update(SDL_Renderer* renderer) {
    for (auto& entity: GetEntities()) {
      const auto transform = entity.GetComponent<TransformComponent>();
      const auto sprite = entity.GetComponent<SpriteComponent>();

      SDL_Rect obj{static_cast<int>(transform.position.coords.x), static_cast<int>(transform.position.coords.y),
        sprite.width, sprite.height};
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
      SDL_RenderFillRect(renderer, &obj);
    }
  }
};

#endif// STABBY2D_RENDERSYSTEM_HPP
