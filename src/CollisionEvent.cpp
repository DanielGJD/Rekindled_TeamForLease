/*!
    \file   CollisionEvent.cpp
    \author Sean McGeer
    \date   11/18/15

    \brief
        Implements a class for a collision event.

    \see CollisionEvent.h

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "CollisionEvent.h"

namespace ForLeaseEngine {

    /*!
        Constructor for a collision entity.

        \param other
            A pointer to the entity that the target entity collided with.
    */
    CollisionEvent::CollisionEvent(Entity* first, Entity* second) : Event("Collision"), Colliding(first, second) {}

    bool CollisionEvent::Matches(Entity* entity) {
        if (entity == Colliding.first || entity == Colliding.second)
            return true;
        else
            return false;
    }

    Entity* CollisionEvent::Other(Entity* entity) {
        if (entity == Colliding.first) return Colliding.second;
        if (entity == Colliding.second) return Colliding.first;
        return NULL;
    }

} // ForLeaseEngine
