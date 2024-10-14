#include "ECS.hpp"
#include "Logger.hpp"

unsigned int Entity::GetId() const { return m_entityId; }

void System::AddEntity(const Entity& entity) {
  Logger::Info("Added entityId " + std::to_string(entity.GetId()) + " to system");
  m_entities.emplace_back(entity);
}

void System::RemoveEntity(Entity &entity) {
    // C++20 way of removing elements from vector
    // no need for erase-remove idiom
    std::erase_if(m_entities, [&entity](Entity& other) {
        return other == entity;
    });
}

std::vector<Entity> System::GetEntities() const{
    return m_entities;
}

Signature const& System::GetComponentSignature() const {
    return m_componentSignature;
}

Entity Registry::CreateEntity() {
    auto entityId = m_numEntities++;
    Entity entity(entityId);
    entity.registry = this;
    m_entitiesToBeAdded.insert(entity);

    if (entityId >= m_entityComponentSignatures.size()) { m_entityComponentSignatures.resize(entityId + 1);
    }
    Logger::Info("Entity created : " + std::to_string(entityId));
    return entity;
}

void Registry::AddEntityToSystems(const Entity& entity) {
  const auto& entityComponentSignature = m_entityComponentSignatures[entity.GetId()];

    // Loop through all m_systems, add entity to the ones whose signature matches entityComponentSignature
    for (const auto& system : m_systems) {
      const auto& systemComponentSignature = system.second->GetComponentSignature();
        if ((entityComponentSignature & systemComponentSignature) == systemComponentSignature) {
            system.second->AddEntity(entity);
        }
    }
}

void Registry::Update() {
    for(const auto& entity: m_entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }
    m_entitiesToBeAdded.clear();
}
