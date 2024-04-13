#include "ECS.hpp"
#include "Logger.hpp"

unsigned int Entity::GetId() const { return id; }

void System::AddEntity(const Entity& entity) {
  Logger::Info("Added entityId " + std::to_string(entity.GetId()) + " to system");
  entities.emplace_back(entity);
}

void System::RemoveEntity(Entity &entity) {
    // C++20 way of removing elements from vector
    // no need for erase-remove idiom
    std::erase_if(entities, [&entity](Entity& other) {
        return other == entity;
    });
}

std::vector<Entity> System::GetEntities() const{
    return entities;
}

Signature const& System::GetComponentSignature() const {
    return componentSignature;
}

Entity Registry::CreateEntity() {
    auto entityId = numEntities++;
    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);

    if (entityId >= entityComponentSignatures.size()) {
      entityComponentSignatures.resize(entityId + 1);
    }
    Logger::Info("Entity created : " + std::to_string(entityId));
    return entity;
}

void Registry::AddEntityToSystems(const Entity& entity) {
  const auto& entityComponentSignature = entityComponentSignatures[entity.GetId()];

    // Loop through all systems, add entity to the ones whose signature matches entityComponentSignature
    for (const auto& system : systems) {
      const auto& systemComponentSignature = system.second->GetComponentSignature();
        if ((entityComponentSignature & systemComponentSignature) == systemComponentSignature) {
            system.second->AddEntity(entity);
        }
    }
}

void Registry::Update() {
    for(const auto& entity: entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();
}
