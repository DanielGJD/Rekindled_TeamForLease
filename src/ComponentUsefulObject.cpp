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
#include "Engine.h"
#include "Entity.h"
#include "ComponentPhysics.h"

namespace ForLeaseEngine {

    namespace Components {

        UsefulObject::UsefulObject(Entity& owner, UsefulObjectCategory category) : Component(owner, ComponentType::Transform | ComponentType::Collision | ComponentType::Physics), Category(category) {}

        UsefulObject* UsefulObject::Create(Entity& owner) {
            UsefulObject* object = new UsefulObject(owner);
            object->Initialize();
            return object;
        }

        void UsefulObject::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "Collision", &UsefulObject::OnCollide, &Parent);
        }

        void UsefulObject::Update() {
            Components::Collision* collision = Parent.GetComponent<Components::Collision>();
            if (!collision->CollidedLastFrame) return;
//            Vector accel;
            Components::Physics* physics = Parent.GetComponent<Components::Physics>();
//            accel[0] = accel[0] - physics->Velocity[0];
//            physics->Acceleration = accel;
            physics->Velocity.x = 0;
        }



        void UsefulObject::OnCollide(const Event* e) {
//            Parent.GetComponent<Components::Physics>()->Velocity.x = 0;
        }

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
