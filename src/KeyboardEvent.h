/*!
    \file   KeyboardEvent.h
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Defines a class for a keyboard event

    \see KeyboardEvent.cpp
*/

#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H

#include "Event.h"

namespace ForLeaseEngine {
    /*!
        \class KeyboardEvent

        \brief
            Class containing information of a change in keyboard state
    */
    class KeyboardEvent : public Event {
        public:
            //! The key that changed state
            int Key;
            //! The state of the key
            int State;

            KeyboardEvent(std::string eventName);
            KeyboardEvent(std::string eventName, int key, int state);
    };
}

#endif // KEYBOARDEVENT_H
