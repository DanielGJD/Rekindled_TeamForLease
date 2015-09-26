/*!
    \file   SystemCollision.h
    \author Sean McGeer
    \date   9/25/15
    \brief
        Defines the Collision system.
    \see SystemCollision.cpp
*/

#ifndef SYSTEM_COLLISION_H
#define SYSTEM_COLLISION_H

#include "ComponentCollision.h"
#include "ComponentTransform.h"
#include "ComponentPhysics.h"
#include "Vector.h"
#include "Entity.h"
#include "System.h"

namespace ForLeaseEngine {
    
    namespace Systems {
        
        class Collision : public System {
            public:
                Collision(Engine& owner);
                void Update(std::vector<Entity *>& entities);
                bool CheckCollision(Entity* entity1, Entity* entity2);
                void ResolveCollision(Entity* entity1, Entity* entity2);
                void ResolveCollisionOneEntityOnly(Entity* toResolve, Entity* other);
        };
        
    }
    
}

#endif
