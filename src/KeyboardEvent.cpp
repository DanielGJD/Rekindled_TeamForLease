/*!
    \file   KeyboardEvent.cpp
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Implementation of a class that holds information about keyboard state changes

    \see KeyboardEvent.h
*/

#include "Keys.h"
#include "Event.h"
#include "KeyboardEvent.h"

namespace ForLeaseEngine {
    /*!
        \brief
            Creates a KeyboardEvent with default data

        \param eventName
            Name of the event being sent
    */
    KeyboardEvent::KeyboardEvent(std::string eventName) : Event(eventName) {}

    /*!
        \brief
            Creates a KeyboardEvent with given data

        \param eventName
            Name of the event being sent

        \param key
            The key whose state changed

        \param state
            The state of the key
    */
    KeyboardEvent::KeyboardEvent(std::string eventName, int key, int state) : Event(eventName), Key(key), State(state) {}
}
