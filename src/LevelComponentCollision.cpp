/*!
    \file   LevelComponentCollision.cpp
    \author Sean McGeer
    \date   9/25/15
    \brief
        Implements the Collision system.
    \see LevelComponentCollision.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "LevelComponentCollision.h"
#include "ComponentsInclude.h"
#include "Component.h"
#include <cmath>
#include "EventDispatcher.h"
#include "Engine.h"
#include "CollisionEvent.h"
#include "HalfPlane.h"
#include "Ray.h"
#include "LevelComponentRenderer.h"
#include "HalfPlane.h"

int i = 0;

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
            for (Entity* entity : entities) {
                if (!CheckEntityCompatibility(entity)) continue;

                entity->GetComponent<Components::Collision>()->CollidedLastFrame = false;
                entity->GetComponent<Components::Collision>()->CollidedWith = 0;
            }

            for (Entity* entity1 : entities) {
                if (!CheckEntityCompatibility(entity1)) continue;

                for (Entity* entity2 : entities) {
                    if (entity2 == entity1 || !CheckEntityCompatibility(entity2)) continue;

                    if (CheckCollision(entity1, entity2)) {
                        CollisionEvent e1 = CollisionEvent(entity1);
                        CollisionEvent e2 = CollisionEvent(entity2);
                        ForLease->Dispatcher.DispatchTo(&e1, entity2);
                        ForLease->Dispatcher.DispatchTo(&e2, entity1);
                        ResolveCollision(entity1, entity2);
                    }

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

            if (entity1Collision->CollidedLastFrame || entity2Collision->CollidedLastFrame) return false;

            entity1Position[0] += entity1Collision->OffsetX;
            entity1Position[1] += entity1Collision->OffsetY;
            entity2Position[0] += entity2Collision->OffsetX;
            entity2Position[1] += entity2Collision->OffsetY;
            bool collided =
                  !(entity2Position[0] - entity2Collision->Width / 2 > entity1Position[0] + entity1Collision->Width / 2 ||
                    entity2Position[0] + entity2Collision->Width / 2 < entity1Position[0] - entity1Collision->Width / 2 ||
                    entity2Position[1] + entity2Collision->Height / 2 < entity1Position[1] - entity1Collision->Height / 2 ||
                    entity2Position[1] - entity2Collision->Height / 2 > entity1Position[1] + entity1Collision->Height / 2);

            if (!collided) return false;

            entity1Collision->CollidedLastFrame = true;
            entity2Collision->CollidedLastFrame = true;

            entity1Collision->CollidedWith = entity2;
            entity2Collision->CollidedWith = entity1;

            return true;

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

            /*return !(entity2Position[0] - entity2Collision->Width / 2 > entity1Position[0] + entity1Collision->Width / 2 ||
                     entity2Position[0] + entity2Collision->Width / 2 < entity1Position[0] - entity1Collision->Width / 2 ||
                     entity2Position[1] + entity2Collision->Height / 2 < entity1Position[1] - entity1Collision->Height / 2 ||
                     entity2Position[1] - entity2Collision->Height / 2 > entity1Position[1] + entity1Collision->Height / 2);*/

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
            Components::Transform* toResolveTransform = toResolve->GetComponent<Components::Transform>(true);
            Components::Physics*   toResolvePhysics   = toResolve->GetComponent<Components::Physics>(true);
            Components::Collision* toResolveCollision = toResolve->GetComponent<Components::Collision>(true);

            Components::Transform* otherTransform = other->GetComponent<Components::Transform>(true);
            Components::Collision* otherCollision = other->GetComponent<Components::Collision>(true);

            Vector velocity = toResolvePhysics->Velocity * ForLease->FrameRateController().GetDt();
            toResolveTransform->Position -= velocity;
            Point toResolvePosition = toResolveTransform->Position;
            //toResolvePosition -= velocity;

            Point toResolveTopLeft(toResolvePosition[0] - toResolveCollision->Width / 2, toResolvePosition[1] + toResolveCollision->Height / 2);
            Point toResolveTopRight(toResolvePosition[0] + toResolveCollision->Width / 2, toResolvePosition[1] + toResolveCollision->Height / 2);
            Point toResolveBotRight(toResolvePosition[0] + toResolveCollision->Width / 2, toResolvePosition[1] - toResolveCollision->Height / 2);
            Point toResolveBotLeft(toResolvePosition[0] - toResolveCollision->Width / 2, toResolvePosition[1] - toResolveCollision->Height / 2);


            Ray toResolveTopLeftRay(toResolveTopLeft, velocity, velocity.Magnitude(), 1);
            Ray toResolveTopRightRay(toResolveTopRight, velocity, velocity.Magnitude(), 1);
            Ray toResolveBotRightRay(toResolveBotRight, velocity, velocity.Magnitude(), 1);
            Ray toResolveBotLeftRay(toResolveBotLeft, velocity, velocity.Magnitude(), 1);

            LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>(true);
            renderer->DrawArrow(toResolveTopLeft, toResolveTopLeftRay.GetScaledVector());
            renderer->DrawArrow(toResolveTopRight, toResolveTopRightRay.GetScaledVector());
            renderer->DrawArrow(toResolveBotRight, toResolveBotRightRay.GetScaledVector());
            renderer->DrawArrow(toResolveBotLeft, toResolveBotLeftRay.GetScaledVector());

            toResolveTopLeftRay.IsColliding(other);
            toResolveTopRightRay.IsColliding(other);
            toResolveBotRightRay.IsColliding(other);
            toResolveBotLeftRay.IsColliding(other);

            Components::Collision::Side side;
            float dist = 9999;

            if (toResolveTopLeftRay.GetLastDistance() < dist && toResolveTopLeftRay.GetLastDistance() > 0.0001f) {
                side = toResolveTopLeftRay.GetLastSide();
                dist = toResolveTopLeftRay.GetLastDistance();
            }

            if (toResolveTopRightRay.GetLastDistance() < dist && toResolveTopRightRay.GetLastDistance() > 0.0001f) {
                side = toResolveTopRightRay.GetLastSide();
                dist = toResolveTopRightRay.GetLastDistance();
            }

            if (toResolveBotRightRay.GetLastDistance() < dist && toResolveBotRightRay.GetLastDistance() > 0.0001f) {
                side = toResolveBotRightRay.GetLastSide();
                dist = toResolveBotRightRay.GetLastDistance();
            }

            if (toResolveBotLeftRay.GetLastDistance() < dist && toResolveBotLeftRay.GetLastDistance() > 0.0001f) {
                side = toResolveBotLeftRay.GetLastSide();
                dist = toResolveBotLeftRay.GetLastDistance();
            }

            toResolvePhysics->Acceleration = Vector(0, 0);
            toResolvePhysics->Velocity = Vector(0, 0);

            //dist -= 0.001f;
            toResolveTransform->Position += velocity * dist;
            dist = 1.0f - dist;
            velocity = velocity * dist;
            toResolveTransform->Position += velocity;

            std::cout << "In Here " << ++i << std::endl;

            //if (side == Components::Collision::Side::Bottom || side == Components::Collision::Side::Top)
            //    velocity[1] = 0.0f;
            //else
            //    velocity[0] = 0.0f;

            //toResolveTransform->Position += velocity;

            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //toResolveTransform->Position[1] -= toResolvePhysics->Velocity[1] * 2 * ForLease->FrameRateController().GetDt();
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //toResolvePhysics->Velocity[1] = 0;
        }

        /*!
            Assumes bounding-box collisions.
        */
        Entity* Collision::GetEntityCollidingAtPoint(std::vector<Entity *>& entities, Point position) {
            Components::Transform* transform;
            Components::Collision* collision;
            for (Entity* entity : entities) {
                if (!entity->HasComponent(ComponentType::Transform)) continue;
                if (!entity->HasComponent(ComponentType::Collision)) continue;
                transform = entity->GetComponent<Components::Transform>();
                collision = entity->GetComponent<Components::Collision>();

                Point entPos = transform->Position;


                Point topLeft(entPos[0] - collision->Width / 2 * transform->ScaleX, entPos[1] + collision->Height / 2 * transform->ScaleY);
                Point topRight(entPos[0] + collision->Width / 2 * transform->ScaleX, entPos[1] + collision->Height / 2 * transform->ScaleY);
                Point botRight(entPos[0] + collision->Width / 2 * transform->ScaleX, entPos[1] - collision->Height / 2 * transform->ScaleY);
                Point botLeft(entPos[0] - collision->Width / 2 * transform->ScaleX, entPos[1] - collision->Height / 2 * transform->ScaleY);

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
