#ifndef ECS_HPP
#define ECS_HPP

#include <cstdint>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <set>
#include <typeindex>
#include <memory>


const uint8_t MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

class IComponent {
    static int nextId;
};

template <typename T>
class Component : public IComponent {
private:
    static int GetId() {
        static const auto id = nextId++;
        return id;
    }
};

class Entity {
private:
    int id;

public:
    // // Rule of five (https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-five)
    Entity(int id) : id{id} {}
    Entity(const Entity& entity) : id{entity.id} {}
    Entity& operator=(const Entity& entity) = default;
    Entity(const Entity&& entity) : Entity(entity) {}
    Entity& operator=(Entity&& entity) = default;

    int GetId() const;
    
    bool operator==(const Entity& other) const { return id == other.id; }
    bool operator!=(const Entity& other) const { return id != other.id; }
    bool operator<(const Entity& other) const { return id < other.id; }
    bool operator>(const Entity& other) const { return id > other.id; }
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

   // Valid entities must have atleast one component
   template <typename T> void RequireComponent();
};

// IPool is pure virtual base class
class IPool {
public:
    virtual ~IPool() {}; // virtual destructor
    IPool(const IPool&) = default; // copy constructor
    IPool& operator=(const IPool&) = default; // copy assignment 
    IPool(IPool&&) = default; // move constructor
    IPool& operator=(IPool&&) = default; // move assignment operator
};

// Pool is a container to store components
template <typename T>
class Pool : public IPool {
private:
   mutable std::vector<T> data;

public:
    // Rule of five (https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-five)
    Pool() = delete; // must not be default constructed without user-specified size

    Pool(const size_t& size) {
        data.reserve(size);
    };

    Pool(Pool& other) : data{other.data} {}

    Pool& operator=(Pool& other) = default;

    Pool(Pool&& other) : Pool(other) {}

    Pool& operator=(Pool&& other) = default;

    ~Pool() = default;
    
    auto IsEmpty() const {
        return data.empty();
    }

    /// @brief Get size of underlying storage
    /// @return size_t
    auto Size() const -> size_t {
        return data.size();
    }

    /// @brief vector::reserve if n is greater than underyling container size, vector::resize otherwise
    /// @param n 
    /// @return void
    auto Resize(size_t n) const -> void {
        if (n >= data.size()) {
            data.reserve(n);
        }
        else {
            data.resize(n);
        }
    }

    auto Clear() -> void {
        data.clear();
    }

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
   mutable size_t numEntities = 0; 
   // each Pool contains the data for certain component type
   mutable std::vector<std::unique_ptr<IPool>> componentPools;
   // tracks which component is turned 'on' (i.e. Signature) per entity.
   mutable std::vector<Signature> entityComponentSignatures;
   // track each system type with map
   std::unordered_map<std::type_index, std::unique_ptr<System>> systems;
   
   // only add/delete entities at the end of game loop
   mutable std::set<Entity> entitiesToBeAdded;
   mutable std::set<Entity> entitiesToBeKilled;

public:
    // Entity management
    Entity CreateEntity();

    // Component management
    template<typename TComponent, typename ...TArgs> void AddComponent(Entity& entity, TArgs&& ...args);
    template<typename TComponent> void RemoveComponent(Entity& entity);
    template<typename TComponent> bool const HasComponent(Entity& entity);

    // System management
    template<typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
    template<typename TSystem> void RemoveSystem();
    template<typename TSystem> bool const HasSystem();
    template<typename TSystem> const TSystem& GetSystem();

    // If an entity's component signature matches to that of a system's required components,
    // add that entity to the system
    void AddEntityToSystems(const Entity& entity);
    void Update();
};

template<typename TComponent>
void System::RequireComponent() {
    auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
};

template<typename TComponent, typename... TArgs>
inline void Registry::AddComponent(Entity &entity, TArgs &&...args) {
    auto const componentId = Component<TComponent>::GetId();
    auto const entityId = entity.GetId();

    // resize if component ID is greater than what componentPools can hold
    if (componentId >= static_cast<decltype(componentId)>(componentPools.size())) {
        componentPools.reserve(componentId * 2);
    }

    // create componentPool for a type if it doesn't exist
    if (!componentPools[componentId]) {
        componentPools[componentId] = std::unique_ptr<TComponent>(new Pool<TComponent>());
    }

    // componentId is guaranteed to exist - fetch componentPool for that componentId
    std::unique_ptr<Pool<TComponent>> componentPool = Pool<TComponent>(componentPools[componentId]);

    if (entityId >= componentPool->Size()) {
        componentPool->Resize(numEntities * 2);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, newComponent);
    entityComponentSignatures[entityId].set(componentId);
};

template<typename TComponent>
inline void Registry::RemoveComponent(Entity &entity) {
    auto const componentId = Component<TComponent>::GetId();
    auto const entityId = entity.GetId();

    entityComponentSignatures[entityId].set(componentId, false);
};

template<typename TComponent>
inline bool const Registry::HasComponent(Entity &entity) { 
    auto const componentId = Component<TComponent>::GetId();
    auto const entityId = entity.GetId();

    return entityComponentSignatures[entityId].test(componentId);
};

template<typename TSystem, typename... TArgs>
inline void Registry::AddSystem(TArgs&& ...args) {
    systems[std::type_index(typeid(TSystem))] = std::make_unique<TSystem>(TSystem(std::forward<TArgs>(args)...));
};

template<typename TSystem>
inline void Registry::RemoveSystem(){
    std::erase_if(systems, [](const auto& item) {
        return item.first == std::type_index(typeid(TSystem));
    });
};

template<typename TSystem>
inline bool const Registry::HasSystem() {
    return systems.count(std::type_index(typeid(TSystem)));
};

template<typename TSystem>
inline const TSystem& Registry::GetSystem() {
    return std::ref(*systems[std::type_index(typeid(TSystem))]);
};

#endif