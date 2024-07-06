#ifndef ECS_HPP
#define ECS_HPP

#include <bitset>
#include <cstdint>
#include <iostream>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "Logger.hpp"

constexpr uint8_t MAX_COMPONENTS = 32;
// Each System needs to keep track of what components are active for that particular system at a point
// This is done using a bitset as a map for component IDs. If in the bitset position i is set it means Component with ID i is set for a System
using Signature = std::bitset<MAX_COMPONENTS>;

class IComponent {
protected:
  inline static unsigned int nextId;
};

template <typename T>
class Component : public IComponent {
public:
  static unsigned int GetId() {
    static const auto componentId = nextId++;
    return componentId;
  }
};

// Fwd declaration for Registry
class Registry;

class Entity {
private:
    uint64_t id;

public:
    // Rule of zero (https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c20-if-you-can-avoid-defining-default-operations-do)
    explicit Entity(uint64_t entityId) : id(entityId) {}

    [[nodiscard]] unsigned int GetId() const;

    bool operator==(const Entity& other) const { return id == other.id; }
    bool operator!=(const Entity& other) const { return id != other.id; }
    bool operator<(const Entity& other) const { return id < other.id; }
    bool operator>(const Entity& other) const { return id > other.id; }

    Registry* registry;
    template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
    template <typename TComponent> void RemoveComponent();
    template <typename TComponent> bool HasComponent() const;
    template <typename TComponent> TComponent& GetComponent();
};

// System processes specific entities
class System {
private:
   Signature componentSignature;
   std::vector<Entity> entities;

public:
   void AddEntity(const Entity& entity);
   void RemoveEntity(Entity& entity);
   std::vector<Entity> GetEntities() const;
   Signature const& GetComponentSignature() const;
   virtual void PrintName() const;

   // Valid entities must have atleast one component
   template <typename TComponent> void RequireComponent();
};

// IPool is abstract base class
class IPool {
public:
    virtual ~IPool() {} // virtual destructor
};

// Pool is a container to store components
template <typename T>
class Pool : public IPool {
private:
  static constexpr int defaultPoolSize = 64;
  std::vector<T> data;

public:
  // Rule of five (https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-five)
  explicit Pool(uint16_t size = defaultPoolSize) {
      data.resize(size);
  };

  Pool(Pool& other) : data{other.data} {}

  Pool& operator=(const Pool& other) = default;

  Pool(Pool&& other) noexcept : Pool(other) {}

  Pool& operator=(Pool&& other) noexcept = default;

  ~Pool() override = default;

  auto IsEmpty() const {
      return data.empty();
  }

  // @brief Get size of underlying storage
  // @return size_t
  [[nodiscard]] auto Size() const -> size_t {
      return data.size();
  }

  // @brief vector::reserve if n is greater than underlying container size, vector::resize otherwise
  // @param n
  // @return void
  auto Resize(size_t n) -> void { data.resize(n); }

  auto Clear() -> void { data.clear(); }

  auto Add(T& object) -> void {
    data.emplace_back(object);
  }

  auto Set(size_t index, T& object) -> void {
      data[index] = object;
  }

  auto Get(size_t index) -> T& {
      return static_cast<T&>(data[index]);
  }

  T& operator [](size_t index) {
      return data[static_cast<int>(index)];
  }
};

// registry class is responsible for creating, removing and tracking entities, components and systems
class Registry {
private:
  size_t numEntities = 0;

  // component IDs are sequential and each element in componentPools points to a Pool of Entities associated with that particular Component.
  std::vector<std::shared_ptr<IPool>> componentPools;

  // tracks which component is turned 'on' (i.e. Signature) per entity.
  std::vector<Signature> entityComponentSignatures;

  // track each system type with map
  std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

  // only add/delete entities at the end of game loop
  std::set<Entity> entitiesToBeAdded;
  std::set<Entity> entitiesToBeKilled;

public:
  // Entity management
  Entity CreateEntity();

  // Component management
  template<typename TComponent, typename ...TArgs> void AddComponent(Entity& entity, TArgs&& ...args);
  template<typename TComponent> void RemoveComponent(Entity& entity);
  template<typename TComponent> bool HasComponent(Entity& entity) const;
  template<typename TComponent> TComponent& GetComponent(Entity& entity) const;

  // System management
  template<typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
  template<typename TSystem> void RemoveSystem();
  template<typename TSystem> bool HasSystem();
  template<typename TSystem> TSystem& GetSystem();

  // If an entity's component signature matches to that of a system's required components,
  // add that entity to the system
  void AddEntityToSystems(const Entity& entity);
  void Update();
};

template<typename TComponent>
void System::RequireComponent() {
  componentSignature.set(Component<TComponent>::GetId());
};

template<typename TComponent, typename... TArgs>
inline void Registry:: AddComponent(Entity &entity, TArgs &&...args) {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  // resize if component ID is greater than what componentPools can hold
  if (componentId >= componentPools.size()) {
    componentPools.resize(componentId + 1, nullptr);
  }

  // create Pool for a Component type if it doesn't exist
  if (!componentPools[componentId]) {
    componentPools[componentId] = std::make_shared<Pool<TComponent>>();
  }

  std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

  if (entityId >= componentPool->Size()) {
    componentPool->Resize(numEntities);
  }

  TComponent newComponent(std::forward<TArgs>(args)...);

  componentPool->Set(entityId, newComponent);

  entityComponentSignatures.resize(componentPools.size());
  entityComponentSignatures[entityId].set(componentId);
};

template<typename TComponent>
inline void Registry::RemoveComponent(Entity &entity) {
  auto const componentId = Component<TComponent>::GetId();
  auto const entityId = entity.GetId();

  entityComponentSignatures[entityId].set(componentId, false);
};

template<typename TComponent>
inline bool Registry::HasComponent(Entity &entity) const {
  auto const componentId = Component<TComponent>::GetId();
  auto const entityId = entity.GetId();

  return entityComponentSignatures[entityId].test(componentId);
};

template<typename TComponent>
TComponent& Registry::GetComponent(Entity &entity) const {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();
  auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
  return componentPool->Get(entityId);
};

template<typename TSystem, typename... TArgs>
inline void Registry::AddSystem(TArgs&& ...args) {
  auto newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
  newSystem->PrintName();
  //systems[std::type_index(typeid(TSystem))] = newSystem;
  systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
};

template<typename TSystem>
inline void Registry::RemoveSystem(){
  std::erase_if(systems, [](const auto& item) -> bool {
      return item.first == std::type_index(typeid(TSystem));
  });
};

template<typename TSystem>
inline bool Registry::HasSystem() {
  return systems.contains(std::type_index(typeid(TSystem)));
};

template<typename TSystem>
inline TSystem& Registry::GetSystem() {
  auto key(std::type_index(typeid(TSystem)));
  return *std::static_pointer_cast<TSystem>(systems[key]);
};

template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&&... args) {
  registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
};

template <typename TComponent>
void Entity::RemoveComponent() {
  registry->RemoveComponent<TComponent>(*this);
};

template<typename TComponent>
bool Entity::HasComponent() const {
  return registry->HasComponent<TComponent>(*this);
};

template <typename TComponent>
TComponent& Entity::GetComponent() {
  return registry->GetComponent<TComponent>(*this);
};

#endif