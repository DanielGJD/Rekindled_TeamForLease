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
                    if (entity2 == entity1 || !CheckEntityCompatibility(entity2)) continue;

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

            if (entity2Position[0] + entity2Collision->Width > entity1Position[0] &&
                entity1Position[0] + entity1Collision->Width > entity2Position[0] &&
                entity2Position[1] + entity2Collision->Height > entity1Position[1] &&
                entity1Position[1] + entity1Collision->Height > entity2Position[1]) {
                    entity1Collision->CollidedLastFrame = true;
                    entity2Collision->CollidedLastFrame = true;
                    std::cout << "Collision detected." << std::endl;
                    return true;
                }

            return false;
        }

        void Collision::ResolveCollision(Entity* entity1, Entity* entity2) {
            Components::Collision* entity1Collision = reinterpret_cast<Components::Collision *>(entity1->GetComponent(ComponentType::Collision));
            Components::Collision* entity2Collision = reinterpret_cast<Components::Collision *>(entity2->GetComponent(ComponentType::Collision));

            std::cout << entity1Collision->ResolveCollisions << " " << entity2Collision->ResolveCollisions << std::endl;

            if (!(entity1Collision->ResolveCollisions) || !(entity2Collision->ResolveCollisions)) return;

            std::cout << "Resolving collision." << std::endl;

            bool entity1HasPhysics = static_cast<bool>(entity1->GetComponentMask() & ComponentType::Physics);
            bool entity2HasPhysics = static_cast<bool>(entity2->GetComponentMask() & ComponentType::Physics);

            // Only one entity has physics--only going to resolve on one entity
            if (entity1HasPhysics != entity2HasPhysics) {
                if (entity1HasPhysics) {
                    ResolveCollisionOneEntityOnly(entity1, entity2);
                    return;
                } else {
                    ResolveCollisionOneEntityOnly(entity2, entity1);
                    return;
                }
            }

            // No physics on either entity--not going to resolve.
            if (entity1HasPhysics == false) return;

            // Both entities have physics.  We'll try to resolve the collision on both.
            // We'll do it stupidlly for now
            ResolveCollisionOneEntityOnly(entity1, entity2);
            ResolveCollisionOneEntityOnly(entity2, entity1);
        }

        void Collision::ResolveCollisionOneEntityOnly(Entity* toResolve, Entity* other) {
            Components::Transform* toResolveTransform = reinterpret_cast<Components::Transform *>(toResolve->GetComponent(ComponentType::Transform));
            Components::Physics*   toResolvePhysics   = reinterpret_cast<Components::Physics   *>(toResolve->GetComponent(ComponentType::Physics));

            //toResolveTransform->Position -= toResolvePhysics->Velocity * 2 * ForLease->FrameRateController().GetDt();
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            toResolveTransform->Position[1] -= toResolvePhysics->Velocity[1] * 2 * ForLease->FrameRateController().GetDt();
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            toResolvePhysics->Velocity[1] = 0;
        }

    } // Systems

} // ForLeaseEngine
