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
    CollisionEvent::CollisionEvent(Entity* with, CollisionSide side) : Event("Collision"), With(with), Side(side) {}

    CollisionStartedEvent::CollisionStartedEvent(Entity* with, CollisionSide side) : Event("CollisionStarted"), With(with), Side(side) {
        std::cout << "Started colliding with " << with->GetName();

        switch (Side) {
            case CollisionSide::Bottom:
                SelfSide = CollisionSide::Top;
                std::cout << " on " << with->GetName() << "'s bottom.";
                break;
            case CollisionSide::Top:
                SelfSide = CollisionSide::Bottom;
                std::cout << " on " << with->GetName() << "'s top.";
                break;
            case CollisionSide::Right:
                SelfSide = CollisionSide::Left;
                std::cout << " on " << with->GetName() << "'s right.";
                break;
            case CollisionSide::Left:
                SelfSide = CollisionSide::Right;
                std::cout << " on " << with->GetName() << "'s left.";
                break;
            default:
                SelfSide = CollisionSide::None;
                break;
        }

        std::cout << std::endl;
    }

    CollisionEndedEvent::CollisionEndedEvent(Entity* with) : Event("CollisionEnded"), With(with) {
        std::cout << "Stopped colliding with " << with->GetName() << std::endl;
    }

} // ForLeaseEngine
