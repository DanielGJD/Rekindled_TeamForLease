/*!
    \file   LevelComponentUsefulObject.h
    \author Sean McGeer
    \date   3/29/16

    \brief
        Implements the useful object level component.

    \see ComponentUsefulObject.h
    \see LevelComponentUsefulObject.h

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "LevelComponentUsefulObject.h"
#include "ComponentUsefulObject.h"
#include <string>

namespace ForLeaseEngine {

    namespace LevelComponents {

        UsefulObject::UsefulObject(State& owner) : LevelComponent(owner, ComponentType::UsefulObject),
            BalloonArchetypeName(""),
            DistractionArchetypeName("") {}

        void UsefulObject::Serialize(Serializer& root) {
            root.WriteUlonglong("Type", static_cast<unsigned long long int>(ComponentType::UsefulObject));
            Serializer usefulObject = root.GetChild("UsefulObject");
            usefulObject.WriteUlonglong("Type", static_cast<unsigned long long int>(ComponentType::UsefulObject));
            usefulObject.WriteString("BalloonArchetypeName", BalloonArchetypeName);
            usefulObject.WriteString("DistractionArchetypeName", DistractionArchetypeName);
            root.Append(usefulObject, "UsefulObject");
        }

        void UsefulObject::Deserialize(Serializer& root) {
            Serializer usefulObject = root.GetChild("UsefulObject");
            usefulObject.ReadString("BalloonArchetypeName", BalloonArchetypeName);
            usefulObject.ReadString("DistractionArchetypeName", DistractionArchetypeName);
        }

    } // LevelComponents

} // ForLeaseEngine