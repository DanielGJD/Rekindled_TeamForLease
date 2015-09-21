#ifndef LISTENER_H
#define LISTENER_H

#include <functional>
#include "Event.h"

namespace ForLeaseEngine {
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
