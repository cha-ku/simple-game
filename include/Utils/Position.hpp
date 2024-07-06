#ifndef POSITION_HPP
#define POSITION_HPP

#include <glm/glm.hpp>
#include "Velocity.hpp"

struct Position {
  float x_pos{};
  float y_pos{};

  Position(const Position& pos)  = default;
  Position(Position&& pos)  = default;
  Position& operator=(const Position& pos) = default;
  Position& operator=(Position&& pos) = default;
  ~Position() = default;

  explicit Position(const glm::vec2 pos) :  x_pos{pos.x}, y_pos{pos.y} {}
  explicit Position()=default;

  Position& operator+=(const glm::vec2& other) {
    x_pos += other.x;
    y_pos += other.y;
    return *this;
  }
};

#endif
