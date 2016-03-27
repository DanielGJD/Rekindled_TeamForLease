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
            CollisionSide Side;

            CollisionEvent(Entity* with, CollisionSide side = CollisionSide::None);

        private:
            CollisionEvent() = delete;
    };

    class CollisionStartedEvent : public Event {
        public:
            Entity* With;
            CollisionStartedEvent(Entity* with);
        private:
            CollisionStartedEvent() = delete;
    };

    class CollisionEndedEvent : public Event {
        public:
            Entity* With;
            CollisionEndedEvent(Entity* with);
        private:
            CollisionEndedEvent() = delete;
    };

} // ForLeaseEngine

#endif // COLLISIONEVENT_H
