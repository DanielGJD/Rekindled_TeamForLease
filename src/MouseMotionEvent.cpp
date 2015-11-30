/*!
    \file   MouseMotionEvent.cpp
    \author Christopher Hudson

    \brief
        Defines a class containing information on mouse movement


    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "MouseMotionEvent.h"

namespace ForLeaseEngine {
    MouseMotionEvent::MouseMotionEvent(std::string eventName) : Event(eventName) {}

    MouseMotionEvent::MouseMotionEvent(std::string eventName, int x, int y, int relativeX, int relativeY)
                                      : Event(eventName),
                                        X(x), Y(y), RelativeX(relativeX), RelativeY(relativeY) {}

    std::ostream& operator<<(std::ostream& os, MouseMotionEvent& rhs) {
        os << "X: " << rhs.X << std::endl
           << "Y: " << rhs.Y << std::endl
           << "Relative X: " << rhs.RelativeX << std::endl
           << "Relative Y: " << rhs.RelativeY;

        return os;
    }
}
