#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Logger/Logger.h"

class MovementSystem: public System
{
public:
    MovementSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }
    void Update(double dt)
    {
        for(auto entity: GetSystemEntities()) 
        {
            auto& transform = entity.GetComponent<TransformComponent>();
            const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

            transform.position.x += rigidbody.velocity.x * dt;
            transform.position.y += rigidbody.velocity.y * dt;
        // Logger::Log("Entity id = " + 
        //     std::to_string(entity.GetID()) +
        //     " position is now ( " + 
        //     std::to_string(transform.position.x) + 
        //     ", " + 
        //     std::to_string(transform.position.y) + ")");
        }
    }
private:
};