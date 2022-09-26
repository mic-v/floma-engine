#include "ECS.h"
#include <algorithm>
#include "../Logger/Logger.h"


int IComponent::nextID = 0;

int Entity::GetID() const 
{
    return this->id;
}


void System::AddEntityToSystem(Entity entity)
{
    entities.push_back(entity);

}


void System::RemoveEntityFromSystem(Entity entity)
{
    //remove entity from the list of entities
    entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {
        return entity == other;
    }), entities.end());
}
std::vector<Entity> System::GetSystemEntities() const
{
    return entities;

}

const Signature& System::GetComponentSignature() const 
{
    return componentSignature;
}

void Registry::AddEntityToSystems(Entity entity)
{
    const auto entityID = entity.GetID();

    const auto& entityComponentSignature = entityComponentSignatures[entityID];

    // Loop all the systems
    for(auto& system: systems)
    {
        const auto& systemComponentSignature = system.second->GetComponentSignature();
        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        if(isInterested)
        {
            system.second->AddEntityToSystem(entity);
        }
    }
}

Entity Registry::CreateEntity() 
{
    int entityID;
    
    entityID = numEntities++;

    Entity entity(entityID);
    entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with a id = " + std::to_string(entityID));

    return entity;
}

void Registry::Update()
{

}