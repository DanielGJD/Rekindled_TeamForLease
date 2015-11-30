/*!
    \file   MouseWheelEvent.cpp
    \author Christopher Hudson

    \brief
        Defines a class that contains information about the mouse scroll wheel

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "MouseWheelEvent.h"

namespace ForLeaseEngine {
    MouseWheelEvent::MouseWheelEvent(std::string EventName) : Event(EventName),
                                     Horizontal(0), Vertical(0) {}

    MouseWheelEvent::MouseWheelEvent(std::string EventName, int horizontal, int vertical) : Event(EventName),
                                     Horizontal(horizontal), Vertical(vertical) {}

    std::ostream& operator<<(std::ostream& os, MouseWheelEvent const& rhs) {
        os << rhs.EventName << ":" << std::endl
           << "Horizontal: " << rhs.Horizontal << std::endl
           << "Vertical: " << rhs.Vertical;

        return os;
    }
}
