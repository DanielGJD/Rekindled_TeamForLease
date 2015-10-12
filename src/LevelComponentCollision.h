/*!
    \file   LevelComponentCollision.h
    \author Sean McGeer
    \date   9/25/15
    \brief
        Defines the Collision system.
    \see LevelComponentCollision.cpp
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
                Collision(State& owner);
                void Update(std::vector<Entity *>& entities);
                bool CheckCollision(Entity* entity1, Entity* entity2);
                void ResolveCollision(Entity* entity1, Entity* entity2);
                void ResolveCollisionOneEntityOnly(Entity* toResolve, Entity* other);
        };
        
    } // LevelComponents
    
} // ForLeaseEngine

#endif
