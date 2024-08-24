#ifndef VELOCITY_HPP
#define VELOCITY_HPP

#include <glm/glm.hpp>

struct Velocity {
public:
  float x;
  float y;

  Velocity(float x_, float y_) : x(x_), y(y_), velocity(x, y) {}
private:
  glm::vec2 velocity;
};

#endif

