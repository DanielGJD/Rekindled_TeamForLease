#ifndef SERIALIZEABLE_H
#define SERIALIZEABLE_H

namespace ForLeaseEngine {
    class Serializeable {
        public:
            virtual void Serialize(Serializer& root) = 0;
            virtual void Deserialize(Serializer& root) = 0;
    };
}

#endif // SERIALIZEABLE_H
