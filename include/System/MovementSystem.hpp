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
  MovementSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<RigidBodyComponent>();
  }

  void Update(double deltaTime) {
    for(auto& entity : GetEntities()) {
      auto& transform = entity.GetComponent<TransformComponent>();
      const auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
      transform.position.coords += rigidBody.velocity.velocity * static_cast<float>(deltaTime);

      Logger::Info("Entity id = " + std::to_string(entity.GetId()) +
                   " position is now (" + std::to_string(transform.position.coords.x) +
                    " , " + std::to_string(transform.position.coords.y) + ")");
    }
  }

};


#endif// STABBY2D_MOVEMENTSYSTEM_HPP
