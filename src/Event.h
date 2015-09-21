#ifndef EVENT_H
#define EVENT_H

#include <string>

namespace ForLeaseEngine {
    class Event {
        public:
            std::string EventName;

            Event(std::string Event) : EventName(Event) {}
    };
}

#endif
