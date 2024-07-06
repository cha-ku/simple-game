//
// Created by chaku on 15/09/23.
//

#ifndef STABBY2D_RENDERSYSTEM_HPP
#define STABBY2D_RENDERSYSTEM_HPP

#include <SDL2/SDL.h>
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"
#include "ECS.hpp"

class RenderSystem : public System {
public:
  explicit RenderSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<SpriteComponent>();
  }
  virtual ~RenderSystem() = default;

  void Update(SDL_Renderer *renderer, std::unique_ptr<AssetManager>& assetStore)
  {
    for (auto& entity: GetEntities()) {
      const auto transform = entity.GetComponent<TransformComponent>();
      const auto sprite = entity.GetComponent<SpriteComponent>();

      SDL_Rect obj{static_cast<int>(transform.position.x), static_cast<int>(transform.position.y),
        static_cast<int>(sprite.width), static_cast<int>(sprite.height)};
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
      SDL_RenderFillRect(renderer, &obj);
    }
  }
};

#endif// STABBY2D_RENDERSYSTEM_HPP
