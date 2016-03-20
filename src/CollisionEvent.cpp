/*!
    \file   CollisionEvent.cpp
    \author Sean McGeer
    \date   11/18/15

    \brief
        Implements a class for a collision event.

    \see CollisionEvent.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "CollisionEvent.h"

namespace ForLeaseEngine {

    /*!
        Constructor for a collision entity.

        \param other
            A pointer to the entity that the target entity collided with.
    */
    CollisionEvent::CollisionEvent(Entity* with, Components::Collision::Side side) : Event("Collision"), With(with), Side(side) {}

} // ForLeaseEngine
