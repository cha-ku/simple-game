#include <algorithm>
#include "ECS.hpp"
#include "../Logger/Logger.hpp"

int Entity::GetId() const { return id; }

void System::AddEntity(Entity &entity) {
    entities.emplace_back(entity);
}

void System::RemoveEntity(Entity &entity) {
    // C++20 way of removing elements from vector
    // no need for erase-remove idiom
    std::erase_if(entities, [&entity](Entity& other) {
        return other == entity;
    });
}

std::vector<Entity> System::GetEntities() const {
    return entities;
}

Signature const& System::GetComponentSignature() const {
    return componentSignature;
}

Entity& Registry::CreateEntity() {
    auto entityId = numEntities++;
    Entity entity(entityId);
    entitiesToBeAdded.insert(entity);

    Logger::Info("Entity added : " + std::to_string(entityId));
    return entity;
}