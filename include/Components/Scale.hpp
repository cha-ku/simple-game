#ifndef SCALE_HPP
#define SCALE_HPP

#include <glm/glm.hpp>

struct Scale {
  float x;
  float y;
  Scale(float xScale, float yScale) : x(xScale), y(yScale), components(x, y) {}
private:
  glm::vec2 components;
};

#endif

