/*!
    \file   MouseWheelEvent.h
    \author Christopher Hudson

    \brief
        Defines a class containing information on mouse wheel events

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef MOUSE_WHEEL_EVENT_H
#define MOUSE_WHEEL_EVENT_H

#include <string>
#include <iostream>
#include "Event.h"

namespace ForLeaseEngine {
    class MouseWheelEvent : public Event {
        public:
            int Horizontal;
            int Vertical;

            MouseWheelEvent(std::string EventName);
            MouseWheelEvent(std::string EventName, int horizontal, int vertical);
            friend std::ostream& operator<<(std::ostream& os, MouseWheelEvent const& rhs);
    };
}

#endif // MOUSE_WHEEL_EVENT_H
