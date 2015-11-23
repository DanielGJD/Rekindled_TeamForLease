/*!
    \file   Serializable.h
    \author Christopher Hudson

    \brief
        Defines a pure virtual class for anything that wants to be serialized

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

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
