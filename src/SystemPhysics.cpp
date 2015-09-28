/*!
    \file   SystemPhysics.cpp
    \author Sean McGeer
    \date   9/24/15
    \brief
        Implements the Physics system.
    \see SystemPhysics.h
*/

#include "SystemPhysics.h"
#include <iostream>

namespace ForLeaseEngine {

    namespace Systems {

        Physics::Physics(Engine& owner, Vector gravity)
            : System(owner, ComponentType::Physics), Gravity(gravity) {}

        void Physics::Update(std::vector<Entity *>& entities) {

            for (Entity* entity : entities) {
                if (CheckEntityCompatibility(entity)) {
                    ApplyGravity(entity);
                    Compute(entity);
                    Cleanup(entity);
                }
            }

        }
        
        void Physics::Compute(Entity* entity) {
            Components::Physics* physicsComponent = reinterpret_cast<Components::Physics *>(entity->GetComponent(ComponentType::Physics));
            Components::Transform* transformComponent = reinterpret_cast<Components::Transform *>(entity->GetComponent(ComponentType::Transform));
        
            physicsComponent->Velocity += physicsComponent->Acceleration * ForLease->FrameRateController().GetDt();
            transformComponent->Position += physicsComponent->Velocity * ForLease->FrameRateController().GetDt();
        }
        
        void Physics::Cleanup(Entity* entity) {
            Components::Physics* physicsComponent = reinterpret_cast<Components::Physics *>(entity->GetComponent(ComponentType::Physics));
            physicsComponent->Acceleration[0] = 0;
            physicsComponent->Acceleration[1] = 0;
            physicsComponent->Force[0] = 0;
            physicsComponent->Force[1] = 0;
        }

        void Physics::ApplyGravity(Entity* entity) {
            Components::Physics* physicsComponent = reinterpret_cast<Components::Physics *>(entity->GetComponent(ComponentType::Physics));

            physicsComponent->Acceleration += Gravity;
        }

        void Physics::SetGravity(Vector gravity) { Gravity = gravity; };

        Vector Physics::GetGravity() { return Gravity; }

    }

}
