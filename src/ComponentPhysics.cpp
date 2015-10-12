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

        /*!
            Constructor for the Physics component.

            \param owner
                The owning Entity.

            \param mass
                A float representing the mass of the object.  Defaults to 1.

            \param velocity
                A vector representing the current velocity of the object.
                Defaults to <0,0>.

            \param acceleration
                A vector represeting the current acceleration on the object.
                Defaults to <0,0>.

            \param force
                A vector representing the current force on the object.
                Defaults to <0,0>.
        */
        Physics::Physics(Entity& owner, float mass, Vector velocity,
            Vector acceleration, Vector force)
            : Component(owner, ComponentType::Transform),
              Mass(mass), Velocity(velocity), Acceleration(acceleration),
              Force(force) {}

    } // Components

} // ForLeaseEngine
