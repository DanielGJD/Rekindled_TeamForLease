#ifndef Serializable_H
#define Serializable_H

#include "Serialize.h"

namespace ForLeaseEngine {
    class Serializable {
        public:
            virtual void Serialize(Serializer& root) = 0;
            virtual void Deserialize(Serializer& root) = 0;
            virtual ~Serializable() {}
    };
}

#endif // Serializable_H
