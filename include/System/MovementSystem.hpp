//
// Created by chaku on 14/09/23.
//

#ifndef STABBY2D_MOVEMENTSYSTEM_HPP
#define STABBY2D_MOVEMENTSYSTEM_HPP

#include "ECS.hpp"
#include "RigidBodyComponent.hpp"
#include "TransformComponent.hpp"

class MovementSystem : public System {
public:
  explicit MovementSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<RigidBodyComponent>();
  }
  virtual ~MovementSystem() = default;

  void Update(float deltaTime) {
    for(auto& entity : GetEntities()) {
      auto& transform = entity.GetComponent<TransformComponent>();
      const auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
      transform.position += rigidBody.velocity * deltaTime;

      Logger::Info("Entity id = " + std::to_string(entity.GetId()) +
                   " position is now (" + std::to_string(transform.position.x) +
                    " , " + std::to_string(transform.position.y) + ")");
    }
  }

  void PrintName() const override {
    Logger::Info("MovementSystem");
  }

};


#endif// STABBY2D_MOVEMENTSYSTEM_HPP
