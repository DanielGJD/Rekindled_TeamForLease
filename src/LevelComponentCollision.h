/*!
    \file   LevelComponentCollision.h
    \author Sean McGeer
    \date   9/25/15
    \brief
        Defines the Collision system.
    \see LevelComponentCollision.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef LC_COLLISION_H
#define LC_COLLISION_H

#include "ComponentCollision.h"
#include "ComponentTransform.h"
#include "ComponentPhysics.h"
#include "Vector.h"
#include "Entity.h"
#include "LevelComponent.h"

namespace ForLeaseEngine {

    namespace LevelComponents {

        /*!
            \class Collision

            \brief
                A Collision level component.  Used to detect and resolve collisions
                between entities in the level.
        */
        class Collision : public LevelComponent {
            public:
                static const ComponentType Type = ComponentType::Collision;
                virtual ComponentType GetType() { return Type; }
                Collision(State& owner, float vertexSpacing = 0.25);
                void Update(std::vector<Entity *>& entities);
                void CheckAndResolveCollision(Entity* entity, std::vector<Entity *>& entities);
                bool CheckCollision(Entity* entity1, Entity* entity2);
                void ResolveCollision(Entity* entity1, Entity* entity2);
                void ResolveCollisionOneEntityOnly(Entity* toResolve, Entity* other);

                Entity* GetEntityCollidingAtPoint(std::vector<Entity *>& entities, Point position);

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);

                void PhysicsCompute(Entity* entity);
                void PhysicsCleanup(Entity* entity);
            private:
                void ResolveCollisionBoundingBox(Entity* toResolve, Entity* other);
                void ResolveCollisionMesh(Entity* toResolve, Entity* other);
                void CheckRay(Point point, Vector velocity, Entity* other, Components::Collision::Side& side, float& dist);
                static const unsigned DefaultVertsPerSide = 2;
                float VertexSpacing;
        };

    } // LevelComponents

} // ForLeaseEngine

#endif
