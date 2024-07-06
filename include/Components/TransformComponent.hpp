#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

#include <glm/glm.hpp>
#include <utility>

struct TransformComponent {
  glm::vec2 position{0, 0};
  glm::vec2 scale{1, 1};
  double rotation{0};

  explicit TransformComponent(glm::vec2 pos=glm::vec2(0, 0), glm::vec2 scl=glm::vec2(1, 1), double rot=0.0) : position(pos), scale(scl), rotation(rot) { }
};

#endif
