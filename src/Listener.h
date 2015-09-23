/*!
    \file   Listener.h
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Defines a class that draws things to the screen

    \see Listener.cpp
*/

#ifndef LISTENER_H
#define LISTENER_H

#include <functional>
#include "Event.h"

namespace ForLeaseEngine {
    /*!
        \class Listener

        \brief
            Contains data on an event listener
    */
    struct Listener {
        Listener(void* ObjectSending,
                 void* ObjectReceiving,
                 std::function<void(const Event*)> ReceivingCallback);
        void* Sender;
        void* Receiver;
        std::function<void(const Event*)> Callback;
    };
}

#endif // LISTENER_H
