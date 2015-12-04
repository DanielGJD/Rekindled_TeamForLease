/*!
    \file   ComponentCharacterController.cpp
    \author Christopher Hudson

    \brief
        Defines the the controls for the main character

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentCharacterController.h"
#include "ComponentPhysics.h"
#include "ComponentSoundEmitter.h"
#include "Entity.h"
#include "Engine.h"

namespace ForLeaseEngine {
    namespace Components {
        CharacterController::CharacterController(Entity& owner)
                                                : Component(owner, ComponentType::Physics | ComponentType::Collision),
                                                  RightKey(Keys::Right), LeftKey(Keys::Left), JumpKey(Keys::Space),
                                                  MoveSpeed(0), JumpSpeed(0), WalkSound(""), JumpSound(""), LandSound(""),
                                                  WalkAnimation(""), JumpAnimation(""), CanJump(false) {};

        CharacterController* CharacterController::Create(Entity& owner) {
            CharacterController* controller = new CharacterController(owner);
            controller->Initialize();
            return controller;
        }

        CharacterController::~CharacterController() {
            ForLease->Dispatcher.Detach(this, "KeyDown");
            ForLease->Dispatcher.Detach(this, "KeyUp");
        }

        void CharacterController::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "KeyDown", &CharacterController::OnKeyDown);
            ForLease->Dispatcher.Attach(NULL, this, "KeyUp", &CharacterController::OnKeyUp);
        }

        void CharacterController::Update() {
            Collision* collider = Parent.GetComponent<Collision>();
            CanJump = false;
            if(collider->CollidedLastFrame)
                CanJump = true;
        };

        void CharacterController::OnKeyDown(const Event* e) {
            const KeyboardEvent* key_e = static_cast<const KeyboardEvent*>(e);
            Components::SoundEmitter* emitter = Parent.GetComponent<Components::SoundEmitter>();
            Components::Collision* collider = Parent.GetComponent<Components::Collision>();
            Components::Model* model = Parent.GetComponent<Components::Model>();
            if(key_e->Key == LeftKey) {
                Physics* rbody = Parent.GetComponent<Physics>();
                rbody->Velocity += Vector(-MoveSpeed, 0);
                if(model)
                    model->FlipY = true;
                if(collider->CollidedLastFrame && collider->CollidedWithSide == Collision::Side::Top) {
                    if(emitter)
                        //emitter->Looping = true;
                        //emitter->Play(WalkSound);
                    if(model)
                        model->SetAnimation(WalkAnimation);
                }
            }
            else if(key_e->Key == RightKey) {
                Physics* rbody = Parent.GetComponent<Physics>();
                rbody->Velocity += Vector(MoveSpeed, 0);
                if(model)
                    model->FlipY = true;
                if(collider->CollidedLastFrame && collider->CollidedWithSide == Collision::Side::Top) {
                    if(emitter)
                        //emitter->Play(WalkSound);
                    if(model)
                        model->SetAnimation(WalkAnimation);
                }
            }
            else if(key_e->Key == JumpKey) {
                Physics* rbody = Parent.GetComponent<Physics>();
                Collision* collider = Parent.GetComponent<Collision>();
                if(CanJump) {
                    rbody->Velocity += Vector(0, JumpSpeed);
                    CanJump = false;
                }
            }
            else if (key_e->Key == Keys::Q) {
                ForLease->GameStateManager().SetAction(Modules::StateAction::Quit);
            }

            // CHEAT CODES //

            else if (key_e->Key == Keys::N)
                ForLease->GameStateManager().SetAction(Modules::StateAction::Next);
            else if (key_e->Key == Keys::M)
                ForLease->GameStateManager().SetAction(Modules::StateAction::Restart);
            else if (key_e->Key == Keys::B) {
                ForLease->GameStateManager().SetState(ForLease->GameStateManager().NumLevels() - 2);
            }
        }

        void CharacterController::OnKeyUp(const Event* e) {
            const KeyboardEvent* key_e = static_cast<const KeyboardEvent*>(e);
            if(key_e->Key == LeftKey) {
                Physics* rbody = Parent.GetComponent<Physics>();
                rbody->Velocity[0] = 0;
            }
            else if(key_e->Key == RightKey) {
                Physics* rbody = Parent.GetComponent<Physics>();
                rbody->Velocity[0] = 0;
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
            controller.WriteString("WalkSound", WalkSound);
            controller.WriteString("JumpSound", JumpSound);
            controller.WriteString("LandSound", LandSound);
            controller.WriteString("WalkAnimation", WalkAnimation);
            controller.WriteString("JumpAnimation", JumpAnimation);
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
            controller.ReadString("WalkSound", WalkSound);
            controller.ReadString("JumpSound", JumpSound);
            controller.ReadString("LandSound", LandSound);
            controller.ReadString("WalkAnimation", WalkAnimation);
            controller.ReadString("JumpAnimation", JumpAnimation);
        }
    }
}
