#ifndef DAMAGE_EVENT_H
#define DAMAGE_EVENT_H

#include "Event.h"

namespace ForLeaseEngine {
    class DamageEvent : public Event {
        public:
            float Damage;

            DamageEvent(float damage);
    };
}

#endif // DAMAGE_EVENT
