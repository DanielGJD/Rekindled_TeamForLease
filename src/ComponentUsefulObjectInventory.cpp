/*!
    \file   ComponentUsefulObjectInventory.cpp
    \author Sean McGeer
    \date   3/29/16
    \brief
        Implements the UsefulObjectInventory component.
    \see ComponentUsefulObjectInventory.h

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentUsefulObjectInventory.h"
#include "State.h"
#include "GameStateManager.h"

namespace ForLeaseEngine {

    namespace Components {

        UsefulObjectInventory* UsefulObjectInventory::Create(Entity& owner) {
            UsefulObjectInventory* object = new UsefulObjectInventory(owner);
            object->Initialize();
            return object;
        }

        UsefulObjectInventory::UsefulObjectInventory(Entity& owner) : Component(owner, ComponentType::Transform | ComponentType::Collision | ComponentType::Physics | ComponentType::Model),
                                                                      Category(UsefulObjectCategory::None),
                                                                      FollowName(""),
                                                                      ThrowKey(Keys::Comma),
                                                                      ThrowVector(20,0) {}

        UsefulObjectInventory::~UsefulObjectInventory() {
            ForLease->Dispatcher.Detach(this, "Collision");
        }

        void UsefulObjectInventory::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "Collision", &UsefulObjectInventory::OnCollide, &Parent);
            ForLease->Dispatcher.Attach(NULL, this, "KeyDown", &UsefulObjectInventory::OnKeyDown);
        }

        void UsefulObjectInventory::Update() {
            if (Category != UsefulObjectCategory::None) {
                Parent.GetComponent<Components::Model>()->ModelColor = Color(1, 0, 0);
            }
            else {
                Parent.GetComponent<Components::Model>()->ModelColor = Color(1, 1, 1);
            }
        }

        void UsefulObjectInventory::OnCollide(const Event* e) {
            const CollisionEvent* collision_e = reinterpret_cast<const CollisionEvent*>(e);

            Entity* with = collision_e->With;
            if (!with) return;
            if (!with->HasComponent(ComponentType::UsefulObject)) return;
            Components::UsefulObject* object = with->GetComponent<Components::UsefulObject>();
            if (!object) return;


            Category = object->Category;

            ForLease->GameStateManager().CurrentState().DeleteEntityAfterFrame(with);

            std::cout << "Picked up ";

            if (Category == UsefulObjectCategory::Balloon)
                std::cout << "balloon" << std::endl;
            else if (Category == UsefulObjectCategory::Distraction)
                std::cout << "distraction" << std::endl;
        }

        void UsefulObjectInventory::OnKeyDown(const Event* e) {
            const KeyboardEvent* key_e = static_cast<const KeyboardEvent*>(e);

            if (key_e->Key != ThrowKey) return;
            if (Category == UsefulObjectCategory::None) return;

            State& currentState = ForLease->GameStateManager().CurrentState();
            LevelComponents::UsefulObject* lcUsefulObject = currentState.GetLevelComponent<LevelComponents::UsefulObject>();
            Entity* newObject = 0;
            std::stringstream name;

            if (Category == UsefulObjectCategory::Balloon) {
                newObject = currentState.SpawnArchetype(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Blueprint) + lcUsefulObject->BalloonArchetypeName);
                name << "Balloon";
            } else if (Category == UsefulObjectCategory::Distraction) {
                newObject = currentState.SpawnArchetype(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Blueprint) + lcUsefulObject->DistractionArchetypeName);
                name << "Distraction";
            }  else return;

            name << newObject->GetID();
            newObject->SetName(name.str());

            newObject->GetComponent<Components::Physics>()->Velocity = ThrowVector;

            Category = UsefulObjectCategory::None;
        }

        void UsefulObjectInventory::Serialize(Serializer& root) {
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            Serializer usefulObjectInventory = root.GetChild("UsefulObjectInventory");
            usefulObjectInventory.WriteInt("Category", static_cast<int>(Category));
            usefulObjectInventory.WriteString("FollowName", FollowName);
            usefulObjectInventory.WriteInt("ThrowKey", ThrowKey);
            usefulObjectInventory.WriteVec("ThrowVector", ThrowVector);
            usefulObjectInventory.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            root.Append(usefulObjectInventory, "UsefulObjectInventory");
        }

        void UsefulObjectInventory::Deserialize(Serializer& root) {
            Serializer usefulObjectInventory = root.GetChild("UsefulObjectInventory");
            int category;
            usefulObjectInventory.ReadInt("Category", category);
            Category = static_cast<UsefulObjectCategory>(category);
            usefulObjectInventory.ReadString("FollowName", FollowName);
            usefulObjectInventory.ReadInt("ThrowKey", ThrowKey);
            usefulObjectInventory.ReadVec("ThrowVector", ThrowVector);
        }

    } // Components

} // ForLeaseEngine
