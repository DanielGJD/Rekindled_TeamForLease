/*!
    \file   SystemCollision.cpp
    \author Sean McGeer
    \date   9/25/15
    \brief
        Implements the Collision system.
    \see SystemCollision.h
*/

#include "SystemCollision.h"

namespace ForLeaseEngine {
    
    namespace Systems {
        
        Collision::Collision(Engine& owner) : System(owner, ComponentType::Collision) {}
        
        /*!
            \bug This is not well done.  Complexity O(n^2).  This will be reworked
            after Engine Proof, and I'll include mesh vertex collision checking
            then.
        */
        void Collision::Update(std::vector<Entity *>& entities) {
            for (Entity* entity1 : entities) {
                if (!CheckEntityCompatibility(entity1)) continue;

                reinterpret_cast<Components::Collision *>(entity1->GetComponent(ComponentType::Collision))->CollidedLastFrame = false;

                for (Entity* entity2 : entities) {
                    if (!CheckEntityCompatibility(entity2)) continue;

                    if(CheckCollision(entity1, entity2))
                        ResolveCollision(entity1, entity2);

                }
            }
        }

        bool Collision::CheckCollision(Entity* entity1, Entity* entity2) {
            Point entity1Position = reinterpret_cast<Components::Transform *>(entity1->GetComponent(ComponentType::Transform))->Position;
            Point entity2Position = reinterpret_cast<Components::Transform *>(entity2->GetComponent(ComponentType::Transform))->Position;
            Components::Collision* entity1Collision = reinterpret_cast<Components::Collision *>(entity1->GetComponent(ComponentType::Collision));
            Components::Collision* entity2Collision = reinterpret_cast<Components::Collision *>(entity2->GetComponent(ComponentType::Collision));

            if (entity2Position[0] + entity2Collision->Width < entity1Position[0] ||
                entity1Position[0] + entity1Collision->Width > entity2Position[0] ||
                entity2Position[1] + entity2Collision->Height > entity1Position[1] ||
                entity1Position[1] + entity1Collision->Height > entity2Position[1])
                    return false;

            entity1Collision->CollidedLastFrame = true;
            entity2Collision->CollidedLastFrame = true;
            return true;
        }

        void Collision::ResolveCollision(Entity* entity1, Entity* entity2) {
            Components::Collision* entity1Collision = reinterpret_cast<Components::Collision *>(entity1->GetComponent(ComponentType::Collision));
            Components::Collision* entity2Collision = reinterpret_cast<Components::Collision *>(entity2->GetComponent(ComponentType::Collision));

            if (!entity1Collision->ResolveCollisions || !entity2Collision->ResolveCollisions) return;
                return;

        }

    } // Systems
    
} // ForLeaseEngine
