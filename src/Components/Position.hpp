#ifndef POSITION_HPP
#define POSITION_HPP

#include <glm/glm.hpp>

struct Position {
  glm::vec2 coords;

  Position() : coords(glm::vec2(0.0, 0.0)) {}
  Position(float coord_x, float coord_y) : coords(glm::vec2(coord_x, coord_y)) {}
  explicit Position(glm::vec2&& coord) : coords(coord) {}
};

#endif
