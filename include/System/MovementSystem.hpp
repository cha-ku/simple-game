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

  void Update(const double deltaTime) {
    for(auto& entity : GetEntities()) {
      auto& transform = entity.GetComponent<TransformComponent>();
      auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
      transform.position.x += rigidBody.velocity.x * static_cast<float>(deltaTime);
      transform.position.y += rigidBody.velocity.y * static_cast<float>(deltaTime);

//      Logger::Info("Entity m_entityId = " + std::to_string(entity.GetId()) +
//                   " position is now (" + std::to_string(transform.position.x) +
//                    " , " + std::to_string(transform.position.y) + ")");
    }
  }

};


#endif// STABBY2D_MOVEMENTSYSTEM_HPP
