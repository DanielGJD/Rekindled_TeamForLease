/*!
    \file   OneEntityEvent.h
    \author Christopher Hudson

    \brief
        Defines a event containing a single entity

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef ONE_ENTITY_EVENT
#define ONE_ENTITY_EVENT

#include "Event.h"

namespace ForLeaseEngine {
    class OneEntityEvent : public Event {
        public:
            unsigned long EntityID;

            OneEntityEvent(std::string eventName);
            OneEntityEvent(std::string eventName, unsigned long entityID);
    };
}

#endif // ONE_ENTITY_EVENT
