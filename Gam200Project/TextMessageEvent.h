#ifndef TEXTMESSAGEEVENT_H
#define TEXTMESSAGEEVENT_H

#include <string>
#include "Event.h"

namespace ForLeaseEngine {
    class TextMessageEvent : public Event {
        public:
            TextMessageEvent() : Event("TextMessageEvent") {}
            std::string EventMessage;
    };
}

#endif // TEXTMESSAGEEVENT_H
