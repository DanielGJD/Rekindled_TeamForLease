/*!
    \file   DamageEvent.cpp
    \author Christopher Hudson

    \brief
        Defines an event for sending damage to an entity

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "DamageEvent.h"

namespace ForLeaseEngine {
    DamageEvent::DamageEvent(float damage, bool kill) : Event("DamageEvent"), Damage(damage), Kill(kill) {}
}
