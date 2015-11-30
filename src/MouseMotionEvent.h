/*!
    \file   MouseMotionEvent.h
    \author Christopher Hudson

    \brief
        Defines a class containing information on mouse motion events

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef MOUSE_MOTION_EVENT_H
#define MOUSE_MOTION_EVENT_H

#include "Event.h"
#include <iostream>

namespace ForLeaseEngine {
    class MouseMotionEvent : public Event {
        public:
            int X;
            int Y;
            int RelativeX;
            int RelativeY;

            MouseMotionEvent(std::string eventName);
            MouseMotionEvent(std::string eventName, int x, int y, int relativeX, int relativeY);
            friend std::ostream& operator<<(std::ostream& os, MouseMotionEvent& rhs);
    };
}

#endif // MOUSE_MOTION_EVENT_H
