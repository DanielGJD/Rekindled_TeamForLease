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
