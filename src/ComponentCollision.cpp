/*!
    \file   ComponentCollision.cpp
    \author Sean McGeer
    \date   9/25/15
    \brief
        Implements the Collision component.
    \see ComponentCollision.h
*/

namespace ForLeaseEngine {
    
    namespace Components {
        
        Collision::Collision(Entity& owner)
            : Component(owner, ComponentType::Collision, ComponentType::Transform) {}
        
    }
    
}