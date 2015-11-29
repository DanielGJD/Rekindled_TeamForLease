/*!
    \file   OneEntityEvent.cpp
    \author Christopher Hudson

    \brief
        Defines a event containing a single entity

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "OneEntityEvent.h"

namespace ForLeaseEngine {
    OneEntityEvent::OneEntityEvent(std::string eventName) : Event(eventName) {}
    OneEntityEvent::OneEntityEvent(std::string eventName, unsigned long entityID) : Event(eventName), EntityID(entityID) {}
}
