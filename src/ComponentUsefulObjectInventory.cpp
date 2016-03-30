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
            ThrowVector(20, 0),
            BalloonMass(0.5f),
            NormalMass(1.0f) {}

        UsefulObjectInventory::~UsefulObjectInventory() {
            ForLease->Dispatcher.Detach(this, "Collision");
        }

        void UsefulObjectInventory::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "Collision", &UsefulObjectInventory::OnCollide, &Parent);
            ForLease->Dispatcher.Attach(NULL, this, "KeyDown", &UsefulObjectInventory::OnKeyDown);
        }

        void UsefulObjectInventory::Update() {
            if (Category == UsefulObjectCategory::Balloon)
                Parent.GetComponent<Components::Physics>()->Mass = BalloonMass;
            else
                Parent.GetComponent<Components::Physics>()->Mass = NormalMass;

            Entity* follow = ForLease->GameStateManager().CurrentState().GetEntityByName(FollowName);
            if (!follow) return;

            LevelComponents::UsefulObject* lcUsefulObject = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::UsefulObject>();
            if (!lcUsefulObject) return;

            if (!follow->HasComponent(ComponentType::Model)) return;

            if (Category == UsefulObjectCategory::Balloon)
                follow->GetComponent<Components::Model>()->ModelMesh = lcUsefulObject->BalloonMesh;
            else if (Category == UsefulObjectCategory::Distraction)
                follow->GetComponent<Components::Model>()->ModelMesh = lcUsefulObject->DistractionMesh;
            else
                follow->GetComponent<Components::Model>()->ModelMesh = "";
            
        }

        void UsefulObjectInventory::OnCollide(const Event* e) {
            if (Category != UsefulObjectCategory::None) return;

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
            if (!lcUsefulObject) return;
            Entity* newObject;
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

            Point spawn;

            Components::Transform* transform = Parent.GetComponent<Components::Transform>();
            Components::Collision* collision = Parent.GetComponent<Components::Collision>();
            Components::Model* model = Parent.GetComponent<Components::Model>();

            Components::Collision* noCollision = newObject->GetComponent<Components::Collision>();
            Vector throwVector = ThrowVector;

            if (model->FlipY) {
                throwVector.x *= -1;
                spawn = collision->TopLeft() + Vector(-noCollision->ScaledWidth(), noCollision->ScaledHeight());
            } else {
                spawn = collision->TopRight() + Vector(noCollision->ScaledWidth(), noCollision->ScaledHeight());
            }

            //std::cout << Parent.GetComponent<Components::Transform>()->Position << " " << spawn << std::endl;
            newObject->GetComponent<Components::Transform>()->Position = spawn;

            newObject->GetComponent<Components::Physics>()->Velocity = throwVector;

            Category = UsefulObjectCategory::None;
        }

        void UsefulObjectInventory::Serialize(Serializer& root) {
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            Serializer usefulObjectInventory = root.GetChild("UsefulObjectInventory");
            usefulObjectInventory.WriteInt("Category", static_cast<int>(Category));
            usefulObjectInventory.WriteString("FollowName", FollowName);
            usefulObjectInventory.WriteInt("ThrowKey", ThrowKey);
            usefulObjectInventory.WriteVec("ThrowVector", ThrowVector);
            usefulObjectInventory.WriteFloat("BalloonMass", BalloonMass);
            usefulObjectInventory.WriteFloat("NormalMass", NormalMass);
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
            usefulObjectInventory.ReadFloat("BalloonMass", BalloonMass);
            usefulObjectInventory.ReadFloat("NormalMass", NormalMass);
        }

    } // Components

} // ForLeaseEngine
