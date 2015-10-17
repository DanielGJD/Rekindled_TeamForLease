#ifndef SERIALIZEABLE_H
#define SERIALIZEABLE_H

#include "Serialize.h"

namespace ForLeaseEngine {
    class Serializeable {
        public:
            virtual void Serialize(Serializer& root) = 0;
            virtual void Deserialize(Serializer& root) = 0;
            virtual ~Serializeable() {}
    };
}

#endif // SERIALIZEABLE_H
