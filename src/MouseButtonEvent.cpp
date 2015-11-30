/*!
    \file   MouseButtonEvent.cpp
    \author Christopher Hudson

    \brief
        Defines a class containing information about click events from the mouse

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "MouseButtonEvent.h"

namespace ForLeaseEngine {
    MouseButtonEvent::MouseButtonEvent(std::string eventName) : Event(eventName) {}

    MouseButtonEvent::MouseButtonEvent(std::string eventName, int button, int state, int clicks, const Point& loc)
                                      : Event(eventName),
                                        Button(button), State(state), Clicks(clicks), ScreenLocation(loc) {}
}
