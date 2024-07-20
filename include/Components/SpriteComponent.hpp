//
// Created by chaku on 15/09/23.
//

#ifndef STABBY2D_SPRITECOMPONENT_HPP
#define STABBY2D_SPRITECOMPONENT_HPP

#include <string>
#include <utility>

struct SpriteComponent {
  std::string name;
  int width{};
  int height{};
  SDL_Rect srcRect{};
};

#endif// STABBY2D_SPRITECOMPONENT_HPP
