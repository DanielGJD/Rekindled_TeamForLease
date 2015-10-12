/*!
    \file   ComponentCollision.cpp
    \author Sean McGeer
    \date   9/25/15
    \brief
        Implements the Collision component.
    \see ComponentCollision.h
*/

#include "ComponentCollision.h"

namespace ForLeaseEngine {
    
    namespace Components {
        
        /*!
            Constructor for the Collision class.

            \param owner
                The entity that uses this Collision component.
        */
        Collision::Collision(Entity& owner)
            : Component(owner, ComponentType::Transform) {}
        
    } // Components
    
} // ForLeaseEngine
