/*!
    \file   MouseButtonEvent.h
    \author Christopher Hudson

    \brief
        Defines a class containing information on mouse buttons events

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef MOUSE_BUTTON_EVENT_H
#define MOUSE_BUTTON_EVENT_H

#include "Vector.h"
#include "Event.h"
#include <string>

namespace ForLeaseEngine {
    class MouseButtonEvent : public Event {
        public:
            int Button;
            int State;
            int Clicks;
            Point ScreenLocation;

            MouseButtonEvent(std::string eventName);
            MouseButtonEvent(std::string eventName, int button, int state, int clicks, const Point& loc);
    };
}

#endif // MOUSE_BUTTON_EVENT
