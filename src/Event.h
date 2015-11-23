/*!
    \file   Event.h
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Defines the base event class

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/
#ifndef EVENT_H
#define EVENT_H

#include <string>

namespace ForLeaseEngine {
    /*!
        \class Event

        \brief
            Base class for events
    */
    class Event {
        public:
            //! Name of the event
            std::string EventName;

            Event(std::string Event) : EventName(Event) {}
    };
}

#endif
