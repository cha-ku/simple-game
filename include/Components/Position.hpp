#ifndef POSITION_HPP
#define POSITION_HPP

#include <glm/glm.hpp>
#include "Velocity.hpp"

struct Position {
  float x{};
  float y{};

  Position(float x_, float y_) : x(x_), y(y_), coords(x_, y_) {}
private:
  glm::vec2 coords;
};

#endif
