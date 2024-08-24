//
// Created by chaku on 15/09/23.
//

#ifndef STABBY2D_RENDERSYSTEM_HPP
#define STABBY2D_RENDERSYSTEM_HPP

#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"
#include "ECS.hpp"
#include "AssetManager.hpp"
#include <SDL2/SDL.h>

class RenderSystem : public System {
public:
  RenderSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<SpriteComponent>();
  }

  void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager)
  {
    for (auto &entity : GetEntities()) {
      const auto transform = entity.GetComponent<TransformComponent>();
      const auto sprite = entity.GetComponent<SpriteComponent>();

      SDL_Rect srcRect = sprite.srcRect;

      // destination SDL_Rect
      auto dstRect = SDL_Rect(static_cast<int>(transform.position.x),
        static_cast<int>(transform.position.y),
        static_cast<int>(static_cast<float>(sprite.width) * transform.scale.x),
        static_cast<int>(static_cast<float>(sprite.height) * transform.scale.y));

      SDL_RenderCopyEx(renderer,
        assetManager->GetTexture(sprite.name),
        &srcRect,
        &dstRect,
        transform.rotation,
        nullptr,
        SDL_FLIP_NONE);
    }
  }
};

#endif// STABBY2D_RENDERSYSTEM_HPP
