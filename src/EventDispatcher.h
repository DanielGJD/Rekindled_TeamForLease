#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <deque>
#include <unordered_map>
#include <string>
#include "Event.h"
#include "Listener.h"

namespace ForLeaseEngine {
    class EventDispatcher {
        public:
            void AddHandler(void* sender, void* receiver, std::string event, std::function<void(const Event*)> callback);
            void RemoveHandler(void* receiver, std::string event);
            void Dispatch(const Event* e, void* sender) const;


        private:
            std::unordered_map<std::string, std::deque<Listener>> EventListeners;
    };
}

#endif // EVENTDISPATCHER_H
