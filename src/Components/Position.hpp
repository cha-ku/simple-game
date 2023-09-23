#ifndef POSITION_HPP
#define POSITION_HPP

#include <glm/glm.hpp>
#include "Velocity.hpp"

struct Position {
  glm::vec2 coords;

  //default
  Position() : coords(glm::vec2(0.0, 0.0)) {}
  //copy constructor
  Position(const Position& other)  : coords(other.coords) {}
  //copy assignment
  Position& operator=(const Position& other) {
    this->coords = other.coords;
    return *this;
  }
  //move constructor
  Position(Position&& other)  : coords(std::move(other.coords)) { other.coords = glm::vec2(); }
  //move assignment
  Position& operator=(Position&& rhs) {
    this->coords = std::move(rhs.coords);
    rhs.coords = glm::vec2();
    return *this;
  }

  explicit Position(float coord_x, float coord_y) : coords(glm::vec2(coord_x, coord_y)) {}
  explicit Position(glm::vec2&& coord) : coords(coord) {}
  Position& operator +(Position& other) {
    this->coords += other.coords;
    return *this;
  }

  Position& operator +(Velocity& other) {
    this->coords += other.velocity;
    return *this;
  }
};

#endif
