/*!
    \file   ComponentCharacterController.cpp
    \author Christopher Hudson

    \brief
        Defines the the controls for the main character

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentCharacterController.h"
#include "ComponentPhysics.h"
#include "Entity.h"
#include "Engine.h"

namespace ForLeaseEngine {
    namespace Components {
        CharacterController::CharacterController(Entity& owner)
                                                : Component(owner, ComponentType::Physics),
                                                  RightKey(Keys::Right), LeftKey(Keys::Left), JumpKey(Keys::Space),
                                                  MoveSpeed(0), JumpSpeed(0), CanJump(false) {};

        CharacterController* CharacterController::Create(Entity& owner) {
            CharacterController* controller = new CharacterController(owner);
            controller->Initialize();
            return controller;
        }

        void CharacterController::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "KeyDown", &CharacterController::OnKeyDown);
            ForLease->Dispatcher.Attach(NULL, this, "KeyUp", &CharacterController::OnKeyUp);
        }

        void CharacterController::Update() {
            Collision* collider = Parent.GetComponent<Collision>();
            if(collider->CollidedLastFrame)
                CanJump = true;
        };

        void CharacterController::OnKeyDown(const Event* e) {
            const KeyboardEvent* key_e = static_cast<const KeyboardEvent*>(e);
            if(key_e->Key == LeftKey) {
                Physics* rbody = Parent.GetComponent<Physics>();
                rbody->Velocity += Vector(-MoveSpeed, 0);
            }
            else if(key_e->Key == RightKey) {
                Physics* rbody = Parent.GetComponent<Physics>();
                rbody->Velocity += Vector(MoveSpeed, 0);
            }
            else if(key_e->Key == JumpKey) {
                Physics* rbody = Parent.GetComponent<Physics>();
                Collision* collider = Parent.GetComponent<Collision>();
                if(CanJump) {
                    rbody->Velocity += Vector(0, JumpSpeed);
                    CanJump = false;
                }
            }
            else if(key_e->Key == Keys::Q) {
                ForLease->GameStateManager().SetAction(Modules::StateAction::Quit);
            }
        }

        void CharacterController::OnKeyUp(const Event* e) {
            const KeyboardEvent* key_e = static_cast<const KeyboardEvent*>(e);
            if(key_e->Key == LeftKey) {
                Physics* rbody = Parent.GetComponent<Physics>();
                rbody->Velocity += Vector(0, 0);
            }
            else if(key_e->Key == RightKey) {
                Physics* rbody = Parent.GetComponent<Physics>();
                rbody->Velocity += Vector(0, 0);
            }
        }

        void CharacterController::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer controller = root.GetChild("CharacterController");
            controller.WriteInt("RightKey", RightKey);
            controller.WriteInt("LeftKey", LeftKey);
            controller.WriteInt("JumpKey", JumpKey);
            controller.WriteFloat("MoveSpeed", MoveSpeed);
            controller.WriteFloat("JumpSpeed", JumpSpeed);
            controller.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(controller, "CharacterController");
        }

        void CharacterController::Deserialize(Serializer& root) {
            Serializer controller = root.GetChild("CharacterController");
            controller.ReadInt("RightKey", RightKey);
            controller.ReadInt("LeftKey", LeftKey);
            controller.ReadInt("JumpKey", JumpKey);
            controller.ReadFloat("MoveSpeed", MoveSpeed);
            controller.ReadFloat("JumpSpeed", JumpSpeed);
        }
    }
}
