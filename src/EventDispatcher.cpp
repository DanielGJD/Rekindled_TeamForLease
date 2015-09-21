#include <deque>
#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>
#include "Event.h"
#include "Listener.h"
#include "EventDispatcher.h"

namespace ForLeaseEngine {
    typedef std::unordered_map<std::string, std::deque<Listener>>::iterator EventListenerIterator;

    void EventDispatcher::AddHandler(void* sender, void* receiver, std::string event, std::function<void(const Event*)> callback) {
        if(EventListeners.find(event) == EventListeners.end()) {
            // EventListenerIterator i = EventListeners.find(event);
            // std::deque<Listener> listenerList = std::get<1>(*i);
            EventListeners.insert(std::make_pair<std::string, std::deque<Listener>>(event.c_str(), std::deque<Listener>()));
        }
        Listener newListener = Listener(sender, receiver, callback);
        EventListeners.at(event).push_back(newListener);
    }

    void EventDispatcher::RemoveHandler(void* receiver, std::string event) {
        std::cout << "Detaching " << receiver << " from " << event << std::endl;
        EventListenerIterator i = EventListeners.find(event);
        if(i != EventListeners.end())
        {
            std::deque<Listener>& listenerList = std::get<1>(*i);
            std::cout << "Found listener list with " << listenerList.size() << " listeners" << std::endl;
            for(std::deque<Listener>::iterator j = listenerList.begin(); j != listenerList.end(); ++j) {
                std::cout << "Checking " << (*j).Receiver << " with " << receiver << std::endl;
                if((*j).Receiver == receiver) {
                    std::cout << "Listener found, erasing..." << std::endl;
                    listenerList.erase(j);
                    break;
                }
            }
        }
    }

    void EventDispatcher::Dispatch(const Event* e, void* sender) const {
        std::cout << "Received " << e->EventName << std::endl;
        if(EventListeners.find(e->EventName) != EventListeners.end()) {
            std::deque<Listener> listeners = EventListeners.at(e->EventName);
            std::cout << "Listeners registered: " << listeners.size() << std::endl;
            for(std::deque<Listener>::iterator i = listeners.begin(); i != listeners.end(); ++i) {
                if((*i).Sender == sender){
                    std::cout << "Sending " << e->EventName << " to " << (*i).Receiver << std::endl;
                    (*i).Callback(e);
                }
            }
        }
    }
}
