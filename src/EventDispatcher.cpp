/*!
    \file   EventDispatcher.cpp
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Implementation of a class to dispatch events to registered listeners

    \see EventDispatcher.h

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/
#include <deque>
#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>
#include "Event.h"
#include "Listener.h"
#include "EventDispatcher.h"
#include "Entity.h"

namespace ForLeaseEngine {
    typedef std::unordered_map<std::string, std::deque<Listener>>::iterator EventListenerIterator;

    /*!
        \brief
            Detaches a listener from an event

        \param receiver
            The object to detach

        \param event
            The event to detach from
    */
    void EventDispatcher::Detach(void* receiver, std::string event) {
        //std::cout << "Detaching " << receiver << " from " << event << std::endl;
        EventListenerIterator i = EventListeners.find(event);
        if(i != EventListeners.end())
        {
            std::deque<Listener>& listenerList = std::get<1>(*i);
            //std::cout << "Found listener list with " << listenerList.size() << " listeners" << std::endl;
            for(std::deque<Listener>::iterator j = listenerList.begin(); j != listenerList.end(); ++j) {
                //std::cout << "Checking " << (*j).Receiver << " with " << receiver << std::endl;
                if((*j).Receiver == receiver) {
                    //std::cout << "Listener found, erasing..." << std::endl;
                    listenerList.erase(j);
                    break;
                }
            }
        }
    }

    /*!
        \brief
            Dispatches an event

        \param e
            The event being sent

        \param sender
            The sender of the event
    */
    void EventDispatcher::Dispatch(const Event* e, void* sender) const {
        //std::cout << "Received " << e->EventName << std::endl;
        //std::cout << "Sending " << e->EventName << " globally" << std::endl;
        if(EventListeners.find(e->EventName) != EventListeners.end()) {
            std::deque<Listener> listeners = EventListeners.at(e->EventName);
            //std::cout << "Listeners registered: " << listeners.size() << std::endl;
            for(std::deque<Listener>::iterator i = listeners.begin(); i != listeners.end(); ++i) {
                if((*i).Sender == NULL || (*i).Sender == sender){
                    //std::cout << "Sending " << e->EventName << " to " << (*i).Receiver << std::endl;
                    (*i).Callback(e);
                }
            }
        }
    }

    void EventDispatcher::DispatchTo(const Event* e, void* receiver) const {
        //std::cout << "Sending " << e->EventName << " to " << receiver << std::endl;
        if(EventListeners.find(e->EventName) != EventListeners.end()) {
            std::deque<Listener> listeners = EventListeners.at(e->EventName);
            for(std::deque<Listener>::iterator i = listeners.begin(); i != listeners.end(); ++i) {
                if((*i).Receiver == receiver) {
                    (*i).Callback(e);
                    break;
                }
            }
        }
    }

    void EventDispatcher::DispatchToParent(const Event* e, void* parent) const {
        //std::cout << "Looking for " << parent << " to give event " << e->EventName << std::endl;
        if(e->EventName.compare("Collision") == 0)
            std::cout << "Sending " << e->EventName << " to parent " << reinterpret_cast<Entity*>(parent)->GetName() << std::endl;
        if(EventListeners.find(e->EventName) != EventListeners.end()) {
            std::deque<Listener> listeners = EventListeners.at(e->EventName);
            //std::cout << "Found " << listeners.size() << " objects listening for event" << std::endl;
            for(std::deque<Listener>::iterator i = listeners.begin(); i != listeners.end(); ++i) {
                if((*i).Parent == parent) {
                    //std::cout << "Found entity, sending event" << std::endl;
                    (*i).Callback(e);
                    break;
                }
            }
        }
        else {
            //std::cout << "No objects listening for event" << std::endl;
        }
    }
}
