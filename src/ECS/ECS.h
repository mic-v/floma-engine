#pragma once

#include <vector>
#include <bitset>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "../Logger/Logger.h"

const unsigned int MAX_COMPONENTS = 32;

/*
* We use a bitset(1s and 0s) to keep a track of which compnants an entity has,
* and also helps keep track of which entities a system is interested in.
*/
using Signature = std::bitset<MAX_COMPONENTS>;

struct IComponent 
{
    protected:
        static size_t nextID;
};

// Used to assign a uniquee id to a component type

template <typename T>
class Component : public IComponent
{
    public: 
        //Retruns the unique if of Component<T>
        static size_t GetID()
        {
            static auto id = nextID++;
            return id;
        }
};

class Entity {
    private:
        int id;

    public:
        Entity(int id): id(id) {};
        Entity(const Entity& entity) = default;
        size_t GetID() const;
       
        Entity& operator =(const Entity& other) = default;
        bool operator ==(const Entity& other) const { return id == other.id; }
        bool operator !=(const Entity& other) const { return id != other.id; }
        bool operator >(const Entity& other) const { return id > other.id; }
        bool operator <(const Entity& other) const { return id < other.id; }

        template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
        template <typename TComponent> void RemoveComponent();
        template <typename TComponent> bool HasComponent() const;
        template <typename TComponent> TComponent& GetComponent() const;
        // Hold a pointer to the entity's owner registry
        class Registry* registry;
};

////////////////////////////////////////////////////////////////////////////////
// System
////////////////////////////////////////////////////////////////////////////////
// The system processes entities that contain a specific signature
////////////////////////////////////////////////////////////////////////////////
class System 
{
    private:
        Signature componentSignature;
        std::vector<Entity> entities;
    public:
        System() = default;
        ~System() = default;

        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);
        std::vector<Entity> GetSystemEntities() const;
        const Signature& GetComponentSignature() const;

        // Defines the component type that entities must have to be considered by the system
        template <typename TComponent> void RequireComponent();
};

template <typename TComponent>
void System::RequireComponent()
{
    const auto componentID = Component<TComponent>::GetID();
    componentSignature.set(componentID);
}

/*
/ Pool
/ A pool is just a vector(contiguous data) of objects of Type T
*/

class IPool 
{
    public:
        virtual ~IPool() {}
};

template<typename T>
class Pool: public IPool
{ 
    public:
        Pool(int size = 100)
        {
            data.resize(size);
        }

        virtual ~Pool() = default;

        bool IsEmpty() const
        {
            return data.empty();
        }

        int GetSize() const 
        {
            return data.size();
        }

        void Resize(int n ) 
        {
            data.resize(n);
        }

        void Clear()
        {
            data.clear();
        }

        void Add(T object)
        {
            data.push_back(object);
        }

        void Set(int index, T object)
        {
            data[index] = object;
        }

        T& Get(int index)
        {
            return static_cast<T&>(data[index]);
        }

        T& operator [](unsigned int index) 
        {
            return data[index];
        }
    private:
        std::vector<T> data;

};

class Registry
{
    public:
        Registry() { Logger::Log("Registry created!"); }

        ~Registry() { Logger::Log("Registry destroyed!"); }

        Entity CreateEntity();
        void KillEntity(Entity entity);


        void Update();

        // Checks component signature of an entity and add entity to the systems
        // that are interested in it
        void AddEntityToSystems(Entity entity);

        // Component management        
        template<typename TComponent, typename ...TArgs> void AddComponent(Entity, TArgs&& ...args);
        template<typename TComponent> void RemoveComponent(Entity entity);
        template<typename TComponent> bool HasComponent(Entity entity) const;
        template<typename TComponent> TComponent& GetComponent(Entity entity) const;

        // System management
        template<typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
        template<typename TSystem> void RemoveSystem();
        template<typename TSystem> bool HasSystem(const std::type_index index) const;
        template<typename TSystem> TSystem& GetSystem() const;

    private:
        int numEntities = 0;

        // Set of entities that are flagged to be added or removed in the next registry Update()
        std::set<Entity> entitiesToBeAdded;
        std::set<Entity> entitiesToBeKilled;

        // Vector of component pools, each pool contains all the data for certain pool
        // vector index = component type id
        // pool index = entity id
        std::vector<std::shared_ptr<IPool>> componentPools;

        // Vector of component signatures
        // The signature lets us know which componanets are turned on in an entity
        // vector index = entity id
        std::vector<Signature> entityComponentSignatures;

        //Map of active systems [index = system type]
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
};



template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) 
{
    const auto componentID = Component<TComponent>::GetID();
    const auto entityID = entity.GetID();

    //IF the component id is greater than the current size, then increase size of the component pool
    if(componentID >= static_cast<size_t>(componentPools.size())) 
    {
        componentPools.resize(componentID + 1, nullptr);
    }

    // If the component pool does not exist for the current component, then create a new pool for this component
    if(!componentPools[componentID]) 
    {
        std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
        componentPools[componentID] = newComponentPool;
    }

    // Get the pool of component values for that component type
    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentID]) ;

    // If the entity id is greater than the current size of the component pool, then resize the pool
    if(entityID >= static_cast<size_t>(componentPool->GetSize()))
    {
        componentPool->Resize(numEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityID, newComponent);

    entityComponentSignatures[entityID].set(componentID);

    Logger::Log("Component id = " + std::to_string(componentID) + " was added to entity id " + std::to_string(entityID));
}

template<typename TComponent> 
void Registry::RemoveComponent(Entity entity)
{
    const auto componentID = Component<TComponent>::GetID();
    const auto entityID = entity.GetID();

    entityComponentSignatures[entityID].set(componentID, false);
}

template<typename TComponent>
bool Registry::HasComponent(Entity entity) const
{
    const auto componentID = Component<TComponent>::GetID();
    const auto entityID = entity.GetID();


    return entityComponentSignatures[entityID].test(componentID);
}

template<typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetID();
	const auto entityId = entity.GetID();
    auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    return componentPool->Get(entityId);
}

template<typename TComponent, typename ...TArgs> 
void Entity::AddComponent(TArgs&& ...args)
{
    registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}
template<typename TComponent> 
void Entity::RemoveComponent()
{
    registry->RemoveComponent<TComponent>(*this);
}
template<typename TComponent> 
bool Entity::HasComponent() const
{
    return registry->HasComponent<TComponent>(*this);  
}
template<typename TComponent> 
TComponent& Entity::GetComponent() const
{
    return registry->GetComponent<TComponent>(*this);
}

// TODO
template <typename TSystem, typename ...TArgs> 
void Registry::AddSystem(TArgs&& ...args)
{
    //const auto systemId  = System<TSystem>::GetID();
    TSystem* newSystem(new TSystem(std::forward<TArgs>(args)...));
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)),newSystem));
}
template<typename TSystem> 
void Registry::RemoveSystem()
{
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(std::type_index(typeid(TSystem)));
}


template<typename TSystem> 
bool Registry::HasSystem(const std::type_index index) const
{
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template<typename TSystem> 
TSystem& Registry::GetSystem() const
{
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}