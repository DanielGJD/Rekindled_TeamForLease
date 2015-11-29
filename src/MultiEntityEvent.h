#ifndef MULTI_ENTITY_EVENT
#define MULTI_ENTITY_EVENT

#include "Event.h"
#include <vector>

namespace ForLeaseEngine {
    class MultiEntityEvent : public Event {
        public:
            std::vector<unsigned long> EntityIDs;
            MultiEntityEvent(std::string eventName);
        private:
    };
}

#endif // MULTI_ENTITY_EVENT
