/*!
    \file   UpdateEvent.h
    \author Christopher Hudson

    \brief
        Defines a class containing information on the duration of the last frame

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef UPDATEEVENT_H
#define UPDATEEVENT_H

#include "Event.h"

namespace ForLeaseEngine {
    class UpdateEvent : public Event {
        public:
            double dt;

            UpdateEvent(double dt);
    };
}

#endif // UPDATEEVENT_H
