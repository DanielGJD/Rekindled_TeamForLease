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

namespace ForLeaseEngine {
    
    namespace Systems {
        
        class Collision : public System {
            public:
                Collision(Engine& owner);
                void Update(std::vector<Entity *> entities); 
        }
        
    }
    
}

#endif