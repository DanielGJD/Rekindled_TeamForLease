/*!
    \file   SystemCollision.cpp
    \author Sean McGeer
    \date   9/25/15
    \brief
        Implements the Collision system.
    \see SystemCollision.h
*/

namespace ForLeaseEngine {
    
    namespace Systems {
        
        Collision::Collision(Engine& owner) : System(owner, ComponentType::Collision) {}
        
    } // Systems
    
} // ForLeaseEngine