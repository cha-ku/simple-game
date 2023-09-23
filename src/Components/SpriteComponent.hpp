//
// Created by chaku on 15/09/23.
//

#ifndef STABBY2D_SPRITECOMPONENT_HPP
#define STABBY2D_SPRITECOMPONENT_HPP

struct SpriteComponent {
  int width = 0;
  int height = 0;

  SpriteComponent()=default;
  explicit SpriteComponent(int w, int h) : width(w) , height(h) {}
};

#endif// STABBY2D_SPRITECOMPONENT_HPP
