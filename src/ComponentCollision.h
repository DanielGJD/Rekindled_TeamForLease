/*!
    \file   ComponentCollision.h
    \author Sean McGeer
    \date   9/25/15
    \brief
        Defines the Collision component.
    \see ComponentCollision.cpp
*/

#ifndef COMPONENT_COLLISION_H
#define COMPONENT_COLLISION_H

#include "Component.h"
#include "Transform.h"

namespace ForLeaseEngine {
    
    namespace Components {
        
        class Collision : public Component {
            public:
                Collision(Entity& owner);
        };
        
    }
    
}

#endif