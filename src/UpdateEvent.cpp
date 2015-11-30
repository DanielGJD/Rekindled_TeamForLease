/*!
    \file   UpdateEvent.cpp
    \author Christopher Hudson

    \brief
        Defines a class containing information on the duration of the last frame

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "UpdateEvent.h"

namespace ForLeaseEngine {
    UpdateEvent::UpdateEvent(double dt) : Event("UpdateEvent"), dt(dt) {}
}
