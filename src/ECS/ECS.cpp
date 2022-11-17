#include "ECS.h"
#include <algorithm>
#include "../Logger/Logger.h"


size_t IComponent::nextID = 0;

size_t Entity::GetID() const 
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
    entity.registry = this;
    entitiesToBeAdded.insert(entity);

    // Make sure the entityComponentSignature vector can accomodate the new entity
    if(entityID >= static_cast<int>(entityComponentSignatures.size())) {
        entityComponentSignatures.resize(entityID + 1);
    }

    Logger::Log("Entity created with a id = " + std::to_string(entityID));

    return entity;
}

void Registry::Update()
{
    for(auto entity: entitiesToBeAdded) 
    {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();

    //TODO: remove entities to be removed from the game.
    for(auto entity: entitiesToBeKilled)
    {
    }
}