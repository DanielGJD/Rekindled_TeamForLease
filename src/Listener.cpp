#include <functional>
#include "Event.h"
#include "Listener.h"

namespace ForLeaseEngine {
    Listener::Listener(void* ObjectSending,
                       void* ObjectReceiving,
                       std::function<void(const Event*)> ReceivingCallback) :
                           Sender(ObjectSending),
                           Receiver(ObjectReceiving),
                           Callback(ReceivingCallback) {}
}
