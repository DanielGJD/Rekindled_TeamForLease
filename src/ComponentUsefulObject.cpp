/*!
    \file   ComponentUsefulObject.cpp
    \author Sean McGeer
    \date   3/29/16
    \brief
        Implements the UsefulObject component.
    \see ComponentUsefulObject.h

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentUsefulObject.h"

namespace ForLeaseEngine {

    namespace Components {

        UsefulObject::UsefulObject(Entity& owner, UsefulObjectCategory category) : Component(owner, ComponentType::Transform | ComponentType::Collision | ComponentType::Physics), Category(category) {}

        //UsefulObject* UsefulObject::Create(Entity& owner) {
        //    UsefulObject* object = new UsefulObject(owner);
        //    object->Initialize();
        //    return object;
        //}

        //void UsefulObject::Initialize() {
        //    ForLease->Dispatcher.Attach(NULL, this, "Collision", &EnemyPace::OnPlayerSeen, &Parent);
        //    //ForLease->Dispatcher.Attach(NULL, this, "ObjectNotSeen", &EnemyPace::OnPlayerNotSeen, &Parent);
        //}

        void UsefulObject::Serialize(Serializer& root) {
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            Serializer usefulObject = root.GetChild("UsefulObject");
            usefulObject.WriteInt("Category", static_cast<int>(Category));
            usefulObject.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            root.Append(usefulObject, "UsefulObject");
        }

        void UsefulObject::Deserialize(Serializer& root) {
            Serializer usefulObject = root.GetChild("UsefulObject");
            int category;
            usefulObject.ReadInt("Category", category);
            Category = static_cast<UsefulObjectCategory>(category);
        }

    } // Components

} // ForLeaseEngine
