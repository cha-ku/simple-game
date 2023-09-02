#ifndef VELOCITY_HPP
#define VELOCITY_HPP

#include <glm/glm.hpp>

struct Velocity {
  glm::vec2 velocity;

  Velocity() : velocity(glm::vec2(0.0, 0.0)) {}
  Velocity(float vel_x, float vel_y) : velocity(glm::vec2(vel_x, vel_y)) {}
  explicit Velocity(glm::vec2&& velocity) : velocity(velocity) {}
};

#endif

