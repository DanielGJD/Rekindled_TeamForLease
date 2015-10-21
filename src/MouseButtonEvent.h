#ifndef MOUSE_BUTTON_EVENT
#define MOUSE_BUTTON_EVENT

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
