/*!
    \file   MultiEntityEvent.h
    \author Christopher Hudson

    \brief
        An event class holding multiple entity ids

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef MULTI_ENTITY_EVENT
#define MULTI_ENTITY_EVENT

#include "Event.h"
#include <vector>

namespace ForLeaseEngine {
    class MultiEntityEvent : public Event {
        public:
            std::vector<unsigned long> EntityIDs;
            MultiEntityEvent(std::string eventName);
        private:
    };
}

#endif // MULTI_ENTITY_EVENT
