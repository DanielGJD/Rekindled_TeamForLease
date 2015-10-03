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
