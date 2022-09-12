#pragma once

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <utility>

const unsigned int MAX_COMPONENTS = 32;

/*
* We use a bitset(1s and 0s) to keep a track of which compnants an entity has,
* and also helps keep track of which entities a system is interested in.
*/
using Signature = std::bitset<MAX_COMPONENTS>;


class Entity
{
    private:
        int id;
    public:
        Entity(int id): id(id) {};
        int GetID() const;

        Entity& operator=(const Entity& other) = default;
        bool operator==(const Entity& other) { return id == other.id; }
        bool operator!=(const Entity& other) { return id == other.id; }
        bool operator>(const Entity& other) { return id == other.id; }
        bool operator<(const Entity& other) { return id == other.id; }

        //...

};

struct IComponent 
{
    protected:
        static int nextID;
};

// Used to assign a uniquee id to a component type

template <typename T>
class Component : public IComponent
{
    //Retruns the unique if of Component<T>
    static int GetID()
    {
        static auto id = nextID++;
        return id;

    }
};

/*
// System
/
// The system prcoesses entities that contain a specific signature
*/

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
        template <typename TComponent>
         void RequireComponent();
};

template <typename TComponent>
void System::RequireComponent()
{
    const auto componentID = Component<TComponent>::GetID();
    componentSignature.set(componentID);
}

/*
/ Pool
/*
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
        Registry() = default;

        Entity CreateEntity();
        void KillEntity(Entity entity);


        void Update();

        // TODO: AddComponentToEntitY<T>(...);
        
        template <typname T, typename ...TArgs> void AddComponent(Entity, TArgs&& ...args);

        void AddEntityToSystem(Entity entity);

        void AddSystem(System system)
        {
            //systems.insert(system);
        }
        void RemoveSystem();
        bool HasSystem(const std::type_index index)
        {
            //return systems.find(index);
        }
        System& GetSystem();
        void AddComponent();
        void RemoveComponent();   

    private:
        std::size_t numEntities = 0;

        // Set of entities that are flagged to be added or removed in the next registry Update()
        std::set<Entity> entitiesToBeAdded;
        std::set<Entity> entitiesToBeKilled;

        // Vector of component pools, each pool contains all the data for certain pool
        // vector index = component type id
        // pool index = entity id
        std::vector<IPool*> componentPools;

        // Vector of component signatures
        // The signature lets us know which componanets are turned on in an entity
        // vector index = entity id
        std::vector<Signature> entityComponentSignature;

        //Map of active systems [index = system type]
        std::unordered_map<std::type_index, System*> systems;
};

template <typename T, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) 
{
    const auto componentID = Component<T>::GetID();
    const auto entityID = entity.GetID();


    //IF the component id is greater than the current size, then increase size of the component pool
    if(componentID >= componentPools.size()) 
    {
        componentPools.resize(componentID + 1, nullptr);
    }

    // If the component pool does not exist for the current component, then create a new pool for this component
    if(!componentPools[componentID]) 
    {
        Pool<T> newComponentPool = new Pool<T>();
        componentPools[componentID] = newComponentPool;
    }

    // Get the pool of component values for that component type
    Pool<T> componentPool = Pool<t>(componentPools[componentID]);

    // If the entity id is greater than the current size of the component pool, then resize the pool
    if(entityID >= componentPool->GetSize()) 
    {
        componentPool->Resize(numEntities);
    }

    T newComponent(std::forward<TARgs>(args)...);

    componentPool->Set(entityID, newComponent);

    entityComponentSignature[entityID].set(componentID);
}

