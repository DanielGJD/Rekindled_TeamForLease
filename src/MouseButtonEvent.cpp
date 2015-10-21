#include "MouseButtonEvent.h"

namespace ForLeaseEngine {
    MouseButtonEvent::MouseButtonEvent(std::string eventName) : Event(eventName) {}

    MouseButtonEvent::MouseButtonEvent(std::string eventName, int button, int state, int clicks, const Point& loc)
                                      : Event(eventName),
                                        Button(button), State(state), Clicks(clicks), ScreenLocation(loc) {}
}
