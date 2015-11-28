/*!
    \file   Listener.h
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Defines a class that stores a callback function, for use with the event dispatcher

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
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
                 std::function<void(const Event*)> ReceivingCallback, void* Parent = NULL);
        void* Sender;
        void* Receiver;
        std::function<void(const Event*)> Callback;
        void* Parent;
    };
}

#endif // LISTENER_H
