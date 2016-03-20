/*!
    \file   CollisionEvent.h
    \author Sean McGeer
    \date   11/18/15

    \brief
        Defines a class for a collision event.

    \see CollisionEvent.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "Event.h"
#include "Entity.h"
#include "ComponentCollision.h"
#include <utility>

namespace ForLeaseEngine {

    /*!
        \class CollisionEvent

        \brief
            Sent to any entity that collides with another entity.  Contains a pointer to the
            entity that the target entity collided with.
    */
    class CollisionEvent : public Event {
        public:
            Entity* With;
            Components::Collision::Side Side;

            CollisionEvent(Entity* with, Components::Collision::Side side = Components::Collision::Side::None);

        private:
            CollisionEvent() = delete;
    };

} // ForLeaseEngine

#endif // COLLISIONEVENT_H
