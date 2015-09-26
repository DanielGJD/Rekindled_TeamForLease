/*!
    \file   ComponentPhysics.cpp
    \author Sean McGeer
    \date   9/24/15
    \brief
        Implements the Physics component.
    \see ComponentPhysics.h
*/

#include "ComponentPhysics.h"
#include "Entity.h"

namespace ForLeaseEngine {

    namespace Components {

        Physics::Physics(Entity& owner, float mass, Vector velocity,
            Vector acceleration, Vector force)
            : Component(owner, ComponentType::Physics, ComponentType::Transform),
              Mass(mass), Velocity(velocity), Acceleration(acceleration),
              Force(force) {}

        }

    } // Components

} // ForLeaseEngine
