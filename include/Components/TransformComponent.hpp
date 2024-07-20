#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

#include "Logger.hpp"
#include "Position.hpp"
#include "Scale.hpp"
#include <glm/glm.hpp>
#include <utility>

using Rotation = double;

struct TransformComponent {
  Position position;
  Scale scale;
  Rotation rotation{0};
};

#endif
