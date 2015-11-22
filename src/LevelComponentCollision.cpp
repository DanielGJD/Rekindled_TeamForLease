/*!
    \file   LevelComponentCollision.cpp
    \author Sean McGeer
    \date   9/25/15
    \brief
        Implements the Collision system.
    \see LevelComponentCollision.h
*/

#include "LevelComponentCollision.h"
#include "ComponentsInclude.h"
#include "Component.h"
#include <cmath>
#include "HalfPlane.h"

namespace ForLeaseEngine {

    namespace LevelComponents {

        /*!
            Constructor for a new Collision LevelComponent.

            \param owner
                The State instance that created this Collision LevelComponent.
        */
        Collision::Collision(State& owner) : LevelComponent(owner, ComponentType::Collision) {}

        /*!
            Update function.  Loops through all entities and checks collisions.

            \param entities
                A reference to a vector of Entity pointers that we want to check
                for collisions on.

            \bug This is not well done.  Complexity O(n^2).  This will be reworked
            after Engine Proof, and I'll include mesh vertex collision checking
            then.
        */
        void Collision::Update(std::vector<Entity *>& entities) {
            for (Entity* entity1 : entities) {
                if (!CheckEntityCompatibility(entity1)) continue;

                entity1->GetComponent<Components::Collision>()->CollidedLastFrame = false;

                for (Entity* entity2 : entities) {
                    if (entity2 == entity1 || !CheckEntityCompatibility(entity2)) continue;

                    if(CheckCollision(entity1, entity2))
                        ResolveCollision(entity1, entity2);

                }
            }
        }

        /*!
            Checks collision between two entities.

            \param Entity1
                A pointer to the first Entity.

            \param Entity2
                A pointer to the second Entity.

            \return
                True if they are colliding, false if not.

            \bug
                Not well done.
        */
        bool Collision::CheckCollision(Entity* entity1, Entity* entity2) {
            Point entity1Position = entity1->GetComponent<Components::Transform>()->Position;
            Point entity2Position = entity2->GetComponent<Components::Transform>()->Position;
            Components::Collision* entity1Collision = entity1->GetComponent<Components::Collision>();
            Components::Collision* entity2Collision = entity2->GetComponent<Components::Collision>();

//            if (entity2Position[0] + entity2Collision->Width/2 > entity1Position[0] &&
//                entity1Position[0] + entity1Collision->Width/2 > entity2Position[0] &&
//                entity2Position[1] + entity2Collision->Height/2 > entity1Position[1] &&
//                entity1Position[1] + entity1Collision->Height/2 > entity2Position[1]) {
//                    entity1Collision->CollidedLastFrame = true;
//                    entity2Collision->CollidedLastFrame = true;
//                    std::cout << "Collision detected." << std::endl;
//                    return true;
//                }
//
//            return false;

//            return !(entity2Position[0] > (entity1Position[0] + entity1Collision->Width / 2) ||
//                    (entity2Position[0] + entity2Collision->Width / 2) < entity1Position[0] ||
//                     entity2Position[1] > (entity1Position[0] + entity1Collision->Height / 2) ||
//                    (entity2Position[1] + entity2Collision->Height / 2) < entity1Position[1]);

//            return (std::abs(entity1Position[0] - entity2Position[0]) * 2 < (entity1Collision->Width + entity2Collision->Width)) &&
//                   (std::abs(entity1Position[1] - entity2Position[1]) * 2 < (entity1Collision->Height + entity2Collision->Height));

            return !(entity2Position[0] - entity2Collision->Width / 2 > entity1Position[0] + entity1Collision->Width / 2 ||
                     entity2Position[0] + entity2Collision->Width / 2 < entity1Position[0] - entity1Collision->Width / 2 ||
                     entity2Position[1] + entity2Collision->Height / 2 < entity1Position[1] - entity1Collision->Height / 2 ||
                     entity2Position[1] - entity2Collision->Height / 2 > entity1Position[1] + entity1Collision->Height / 2);

        }

        /*!
            Resolves collisions between two objects.

            \param entity1
                The first entity to resolve collisions for.

            \param entity2
                The second entity to resolve collisions for.

            \bug
                Not well done.
        */
        void Collision::ResolveCollision(Entity* entity1, Entity* entity2) {
            Components::Collision* entity1Collision = entity1->GetComponent<Components::Collision>();
            Components::Collision* entity2Collision = entity2->GetComponent<Components::Collision>();

//            std::cout << entity1Collision->ResolveCollisions << " " << entity2Collision->ResolveCollisions << std::endl;

            if (!(entity1Collision->ResolveCollisions) || !(entity2Collision->ResolveCollisions)) return;

//            std::cout << "Resolving collision." << std::endl;

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
            // We'll do it stupidly for now
            ResolveCollisionOneEntityOnly(entity1, entity2);
            ResolveCollisionOneEntityOnly(entity2, entity1);
        }

        /*!
            Resolves collisions on one entity.

            \param toResolve
                A pointer to the Entity to resolve collisions on.

            \param other
                A pointer that toResolve is colliding with.
        */
        void Collision::ResolveCollisionOneEntityOnly(Entity* toResolve, Entity* other) {
            Components::Transform* toResolveTransform = toResolve->GetComponent<Components::Transform>();
            Components::Physics*   toResolvePhysics   = toResolve->GetComponent<Components::Physics>();

            //toResolveTransform->Position -= toResolvePhysics->Velocity * 2 * ForLease->FrameRateController().GetDt();
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            toResolveTransform->Position[1] -= toResolvePhysics->Velocity[1] * 2 * ForLease->FrameRateController().GetDt();
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            toResolvePhysics->Velocity[1] = 0;
        }

        /*!
            Assumes bounding-box collisions.
        */
        Entity* GetEntityCollidingAtPoint(std::vector<Entity *>& entities, Point position) {
            Components::Transform* transform;
            Components::Collision* collision;
            for (Entity* entity : entities) {
                if (!entity->HasComponent(ComponentType::Transform)) continue;
                if (!entity->HasComponent(ComponentType::Collision)) continue;
                transform = entity->GetComponent<Components::Transform>();
                collision = entity->GetComponent<Components::Collision>();

                Point entPos = transform->Position;


                Point topLeft(entPos[0] - collision->Width / 2, entPos[1] + collision->Height / 2);
                Point topRight(entPos[0] + collision->Width / 2, entPos[1] + collision->Height / 2);
                Point botRight(entPos[0] + collision->Width / 2, entPos[1] - collision->Height / 2);
                Point botLeft(entPos[0] - collision->Width / 2, entPos[1] - collision->Height / 2);

                HalfPlane top(topLeft, topRight, entPos);
                HalfPlane right(topRight, botRight, entPos);
                HalfPlane left(topLeft, botLeft, entPos);
                HalfPlane bot(botLeft, botRight, entPos);

                if (right.Dot(position) < 0 && bot.Dot(position) < 0 && left.Dot(position) < 0 && top.Dot(position) < 0)
                    return entity;
            }

            return 0;
        }

        void Collision::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(ComponentType::Collision));
            Serializer collision = root.GetChild("Collision");
            collision.WriteUint("Type", static_cast<unsigned>(ComponentType::Collision));
            root.Append(collision, "Collision");
        }

        void Collision::Deserialize(Serializer& root) {
            Serializer collision = root.GetChild("Collision");
        }

    } // LevelComponents

} // ForLeaseEngine
