/*!
    \file   CollisionEvent.cpp
    \author Sean McGeer
    \date   11/18/15

    \brief
        Implements a class for a collision event.

    \see CollisionEvent.h
*/

#include "CollisionEvent.h"

namespace ForLeaseEngine {

    /*!
        Constructor for a collision entity.

        \param other
            A pointer to the entity that the target entity collided with.
    */
    CollisionEvent::CollisionEvent(Entity* other) : Event("Collision"), Other(other) {}

} // ForLeaseEngine