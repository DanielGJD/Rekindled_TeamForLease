/*!
    \file   EventDispatcher.h
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Defines a class that dispatches events to registered listeners

    \see EventDispatcher.cpp
*/
#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <deque>
#include <unordered_map>
#include <string>
#include <functional>
#include "Event.h"
#include "Listener.h"

namespace ForLeaseEngine {
    /*!
        \class EventDispatcher

        \brief
            Class that allows objects to register as listeners and dispatches events to the listeners
    */
    class EventDispatcher {
        public:
            //void AddHandler(void* sender, void* receiver, std::string event, std::function<void(const Event*)> callback);
            template<typename R, typename F>
            void Attach(void* sender, R* receiver, std::string event, F function) {
                if(EventListeners.find(event) == EventListeners.end()) {
                    // EventListenerIterator i = EventListeners.find(event);
                    // std::deque<Listener> listenerList = std::get<1>(*i);
                    EventListeners.insert(std::make_pair<std::string, std::deque<Listener>>(event.c_str(), std::deque<Listener>()));
                }
                Listener newListener = Listener(sender, receiver, std::bind(function, receiver, std::placeholders::_1));
                EventListeners.at(event).push_back(newListener);
            }
            void Detach(void* receiver, std::string event);
            void Dispatch(const Event* e, void* sender) const;
            void DispatchTo(const Event* e, void* receiver) const;


        private:
            //! Collection of registered listeners
            std::unordered_map<std::string, std::deque<Listener>> EventListeners;
    };
}

#endif // EVENTDISPATCHER_H
