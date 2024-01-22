//
// Created by chaku on 15/09/23.
//

#ifndef STABBY2D_SPRITECOMPONENT_HPP
#define STABBY2D_SPRITECOMPONENT_HPP

#include <string>

struct SpriteComponent {
  int width;
  int height;
  std::string name;

  SpriteComponent()=default;
  explicit SpriteComponent(std::string_view name_, int width_, int height_) : width(width_), height(height_) , name(name_) {}
};

#endif// STABBY2D_SPRITECOMPONENT_HPP
