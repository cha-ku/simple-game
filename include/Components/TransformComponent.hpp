#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

#include "../Logger/Logger.hpp"
#include "Position.hpp"
#include "Scale.hpp"
#include <glm/glm.hpp>

using Rotation = double;

struct TransformComponent {
  Position position;
  Scale scale;
  Rotation rotation = 0;

  TransformComponent()=default;
  explicit TransformComponent(Position position , Scale scale, Rotation rotation) : position(position), scale(scale), rotation(rotation) {}
};

#endif
