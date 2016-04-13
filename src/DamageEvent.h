#ifndef DAMAGE_EVENT_H
#define DAMAGE_EVENT_H

#include "Event.h"

namespace ForLeaseEngine {
    class DamageEvent : public Event {
        public:
            float Damage;
            bool Kill;

            DamageEvent(float damage, bool kill = false);
    };
}

#endif // DAMAGE_EVENT
