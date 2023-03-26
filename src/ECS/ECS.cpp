#include <algorithm>
#include "ECS.hpp"
#include "../Logger/Logger.hpp"

int Entity::GetId() const { return id; }

void System::AddEntity(const Entity& entity) {
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

Entity Registry::CreateEntity() {
    auto entityId = numEntities++;
    Entity entity(entityId);
    entitiesToBeAdded.insert(std::move(entity));

    Logger::Info("Entity added : " + std::to_string(entityId));
    return entity;
}

void Registry::AddEntityToSystems(const Entity& entity) {
    const auto& entityComponentSignature = entityComponentSignatures[entity.GetId()];

    // Loop through all systems, add entity to the ones whose signature matches entityComponentSignature
    for (auto& system : systems) {
        if (entityComponentSignature == system.second->GetComponentSignature()) {
            system.second->AddEntity(entity);
        }
    }
}

void Registry::Update() {
    for(const auto& entity: entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }
}
