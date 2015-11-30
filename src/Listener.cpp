/*!
    \file   Listener.cpp
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Implementation of a class to hold information about an object listening to another object

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved

    \see Listener.h
*/

#include <functional>
#include "Event.h"
#include "Listener.h"

namespace ForLeaseEngine {
    /*!
        \brief
            Creates a listener object

        \param ObjectSending
            The object being listened to

        \param ObjectReceiving
            The object that's listening

        \param ReceivingCallback
            The callback function for the listener

        \param Parent
            The Parent of the listener
    */
    Listener::Listener(void* ObjectSending,
                       void* ObjectReceiving,
                       std::function<void(const Event*)> ReceivingCallback, void* Parent) :
                           Sender(ObjectSending),
                           Receiver(ObjectReceiving),
                           Callback(ReceivingCallback),
                           Parent(Parent) {}
}
