#ifndef ECS_HPP
#define ECS_HPP

#include <cstdint>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <set>
#include <typeindex>


const uint8_t MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

class IComponent {
    static int nextId;
};

template <typename T>
class Component : public IComponent {
private:
    static int GetId() {
        static auto id = nextId++;
        return id;
    }
};

class Entity {
private:
    int id;

public:
    Entity(int id) : id(id) {};
    int GetId() const;
    bool const operator==(Entity const& other) { return id == other.id; }
    bool const operator!=(Entity const& other) { return id != other.id; }
};

// System processes specific entities
class System {

private:
   Signature componentSignature;
   std::vector<Entity> entities; 

public:
   System() = default;
   ~System() = default; 

   void AddEntity(Entity& entity);
   void RemoveEntity(Entity& entity);
   std::vector<Entity> GetEntities() const;
   Signature const& GetComponentSignature() const;

   //Valid entities must have atleast one component
   template <typename T> void RequireComponent();
};

// IPool is pure virtual base class
class IPool {
    public:
    virtual ~IPool() {};
};

// Pool is a container to store components
template <typename T>
class Pool : public IPool {
private:
   std::vector<T> data;

public:
    Pool(size_t size = 64) {
        data.resize(size);
    }

    ~Pool = default;

    auto IsEmpty() const {
        return data.empty();
    }

    auto GetSize() const {
        return data.size();
    }

    auto Resize(size_t n) const {
        data.resize(n);
    }

    auto Clear() {
        data.clear();
    }

    auto Add(T& object) {
        data.emplace_back(object);
    }

    auto Set(size_t index, T& object) {
        data[index] = object;
    }

    T& Get(size_t index) {
        return static_cast<T&>(data[index]);
    }

    T& operator [](size_t index) {
        return data[index];
    }
};

// registry class is responsible for creating, removing and tracking entities, components and systems
class Registry {
private:
   size_t numEntities = 0; 
   // each Pool contains the data for certain component type
   std::vector<IPool*> componentPools;
   // tracks which component is turned 'on' (i.e. Signature) per entity.
   std::vector<Signature> entityComponentSignatures;
   // track each system type with map
   std::unordered_map<std::type_index, System*> systems;
   
   // Only add/delete entities at the end of game loop
   std::set<Entity> entitiesToBeAdded;
   std::set<Entity> entitiesToBeKilled;

public:
    Registry() = default;
    Entity& CreateEntity();
};

template <typename TComponent>
void System::RequireComponent() {
    auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

#endif