#ifndef SCALE_HPP
#define SCALE_HPP

#include <glm/glm.hpp>

struct Scale {
  glm::vec2 coords;

  Scale() : coords(glm::vec2(0.0, 0.0)) {}
  Scale(float coord_x, float coord_y) : coords(glm::vec2(coord_x, coord_y)) {}
  explicit Scale(glm::vec2&& coords) : coords(coords) {}
};

#endif

