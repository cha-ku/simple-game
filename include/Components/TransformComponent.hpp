#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

#include "Logger.hpp"
#include "Position.hpp"
#include "Scale.hpp"
#include <glm/glm.hpp>
#include <utility>

using Rotation = double;

struct TransformComponent {
  Position position{0.0F, 0.0F};
  Scale scale{1.0F, 1.0F};
  Rotation rotation{0};
};

#endif
