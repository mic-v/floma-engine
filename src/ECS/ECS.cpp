#include "ECS.h"
#include <algorithm>
#include "../Logger/Logger.h"


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

Entity Registry::CreateEntity() 
{
    int entityID = numEntities++;
    if(entity >= entityComponentSignature.size())
        entityComponentSignature.resize(entityID + 1);
    Entity entity(entityID);
    entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with a id = " + std::to_string(entityID));

    return entity;
}

void Registry::Update()
{

}

void Registry::AddEntityToSystem(Entity entity);