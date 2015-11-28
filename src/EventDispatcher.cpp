/*!
    \file   EventDispatcher.cpp
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Implementation of a class to dispatch events to registered listeners

    \see EventDispatcher.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/
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
        if(EventListeners.find(e->EventName) != EventListeners.end()) {
            std::deque<Listener> listeners = EventListeners.at(e->EventName);
            for(std::deque<Listener>::iterator i = listeners.begin(); i != listeners.end(); ++i) {
                if((*i).Parent == parent) {
                    (*i).Callback(e);
                    break;
                }
            }
        }
    }
}
