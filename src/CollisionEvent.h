/*!
    \file   CollisionEvent.h
    \author Sean McGeer
    \date   11/18/15

    \brief
        Defines a class for a collision event.

    \see CollisionEvent.cpp

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "Event.h"
#include "Entity.h"
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
            typedef std::pair<Entity*, Entity*> EntityPair;
            
            EntityPair Colliding;

            CollisionEvent(Entity* first, Entity* second);

        private:
            CollisionEvent() = delete;
    };

} // ForLeaseEngine

#endif // COLLISIONEVENT_H