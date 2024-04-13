#ifndef RIGIDBODYCOMPONENT_HPP
#define RIGIDBODYCOMPONENT_HPP

#include "Velocity.hpp"

struct RigidBodyComponent {
  Velocity velocity;

  RigidBodyComponent()=default;
  explicit RigidBodyComponent(Velocity&& velocity) : velocity(velocity) {}
};

#endif
