/*!
    \file   ComponentCharacterController.cpp
    \author Christopher Hudson

    \brief
        Defines the the controls for the main character

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentCharacterController.h"
#include "ComponentPhysics.h"
#include "SoundEmitter.h"
#include "Entity.h"
#include "Engine.h"
#include "CollisionEvent.h"
#include "ComponentCollision.h"

namespace ForLeaseEngine {
    namespace Components {
        CharacterController::CharacterController(Entity& owner)
                                                : Component(owner, ComponentType::Physics | ComponentType::Collision),
                                                  RightKey(Keys::D), LeftKey(Keys::A), JumpKey(Keys::Space),
                                                  Acceleration(20.0f), JumpSpeed(5.0f), Drag(5.0f),WalkSound(""), JumpSound(""), LandSound(""),
                                                  WalkAnimation(""), JumpAnimation(""), CanJump(false), RightPressed(false), LeftPressed(false), LastAnimationFrame(0), Timer(0), JumpSoundTimer(0) {};

        CharacterController* CharacterController::Create(Entity& owner) {
            CharacterController* controller = new CharacterController(owner);
            controller->Initialize();
            return controller;
        }

        CharacterController::~CharacterController() {
            ForLease->Dispatcher.Detach(this, "KeyDown");
            ForLease->Dispatcher.Detach(this, "KeyUp");
            ForLease->Dispatcher.Detach(this, "CollisionStarted");
            ForLease->Dispatcher.Detach(this, "CollisionEnded");
        }

        void CharacterController::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "KeyDown", &CharacterController::OnKeyDown);
            ForLease->Dispatcher.Attach(NULL, this, "KeyUp", &CharacterController::OnKeyUp);
            //ForLease->Dispatcher.Attach(NULL, this, "Collision", &CharacterController::OnCollision, &Parent);
            ForLease->Dispatcher.Attach(NULL, this, "CollisionStarted", &CharacterController::OnCollisionStarted, &Parent);
            ForLease->Dispatcher.Attach(NULL, this, "CollisionEnded", &CharacterController::OnCollisionEnded, &Parent);
        }

        void CharacterController::Update() {
//            std::cout << "Animation: " << Parent.GetComponent<Components::Model>()->GetAnimation() << std::endl;
            //std::cout << "Walk ani: " << WalkAnimation << std::endl;
            Components::SoundEmitter* emitter = Parent.GetComponent<Components::SoundEmitter>();
//            if(emitter && RightPressed != LeftPressed) {
//                Timer += ForLease->FrameRateController().GetDt();
//                //std::cout << "Timer: " << Timer << std::endl;
//                if(Timer >= 0.3) {
//                    emitter->SetVolume(1.0f, WalkSound);
//                    emitter->StopEvent(WalkSound);
//                    emitter->PlayEvent(WalkSound);
//                    Timer = 0;
//                }
//                else if(Timer >= 0.25) {
//                    emitter->StopEvent(WalkSound);
//                }
//            }
//            else if(emitter) {
//                emitter->StopEvent(WalkSound);
//            }
            Collision* collider = Parent.GetComponent<Collision>();
            Physics* physics = Parent.GetComponent<Physics>();
            bool couldJump = CanJump;
            //CanJump = false;
            //if(collider->CollidedLastFrame && collider->CollidedWithSide == CollisionSide::Top)
                //CanJump = true;

            if(couldJump) {
                JumpSoundTimer = 0;
            }
            else {
                JumpSoundTimer += ForLease->FrameRateController().GetDt();
            }
            //std::cout << JumpSoundTimer << std::endl;

            Vector accel;
            if(RightPressed) {
                accel = accel + Vector(Acceleration, 0);
            }
            if(LeftPressed) {
                accel = accel - Vector(Acceleration, 0);
            }
            if(LeftPressed || RightPressed) {
                Components::Model* model = Parent.GetComponent<Components::Model>();
                if(model && model->GetAnimation().compare(WalkAnimation) != 0 && CanJump) {
                    model->AnimationActive = true;
                    model->Looping = true;
                    model->SetAnimation(WalkAnimation);
                }
                if(model && accel[0] > 0.0001) {
                    model->FlipY = false;
                }
                else if(model && accel[0] < -0.0001) {
                    model->FlipY = true;
                }
            }
            else if(CanJump){
                Components::Model* model = Parent.GetComponent<Components::Model>();
                if(model && model->GetAnimation().compare("") != 0) {
                    model->AnimationActive = true;
                    model->Looping = true;
                    model->SetAnimation("");
                }
            }

            accel[0] = accel[0] - physics->Velocity[0] * Drag;
            physics->Acceleration = accel;
            if(!couldJump && CanJump && JumpSoundTimer > 0.1) {
                SoundEmitter* emitter = Parent.GetComponent<SoundEmitter>();
                if(emitter){
                    emitter->SetVolume(1.0f, LandSound);
                    emitter->StopEvent(LandSound);
                    emitter->PlayEvent(LandSound);
                }
            }

        };

        void CharacterController::OnKeyDown(const Event* e) {
            const KeyboardEvent* key_e = static_cast<const KeyboardEvent*>(e);
            Components::SoundEmitter* emitter = Parent.GetComponent<Components::SoundEmitter>();
            Components::Collision* collider = Parent.GetComponent<Components::Collision>();
            Components::Model* model = Parent.GetComponent<Components::Model>();
            Physics* rbody = Parent.GetComponent<Physics>();
            if(key_e->Key == LeftKey)
            {
                LeftPressed = true;
            }
            else if(key_e->Key == RightKey)
            {
                RightPressed = true;
            }
            else if(key_e->Key == JumpKey) {
                Physics* Jbody = Parent.GetComponent<Physics>();
                if(CanJump) {
                    rbody->Velocity[1] = JumpSpeed;
                    if(model) {
                        model->SetAnimation(JumpAnimation);
                        model->Looping = false;
                    }
                    SoundEmitter* emitter = Parent.GetComponent<SoundEmitter>();

                    if (emitter) {
                        emitter->SetVolume(1.0f, JumpSound);
                        emitter->StopEvent(JumpSound);
                        emitter->PlayEvent(JumpSound);
                    }
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
                ForLease->GameStateManager().SetState(ForLease->GameStateManager().NumLevels() - 3);
            }
        }

        void CharacterController::OnKeyUp(const Event* e) {
            const KeyboardEvent* key_e = static_cast<const KeyboardEvent*>(e);
            Components::SoundEmitter* emitter = Parent.GetComponent<Components::SoundEmitter>();
            Components::Model* model = Parent.GetComponent<Components::Model>();
          //  float deccelerate = 0.2;
            if(key_e->Key == LeftKey) {
                LeftPressed = false;

                //if(emitter)
                    //emitter->SetPause(true, WalkSound);

                //if(model)
                    //model->SetAnimation("");
            }
            else if(key_e->Key == RightKey) {
                RightPressed = false;

//                if(emitter)
//                    emitter->SetPause(true, WalkSound);
//                if(model)
//                    model->SetAnimation("");
            }
        }

        void CharacterController::OnCollisionStarted(const Event* e) {
            const CollisionStartedEvent* collision_e = reinterpret_cast<const CollisionStartedEvent*>(e);
            if(collision_e->SelfSide == CollisionSide::Bottom) {
                //std::cout << "Collided with bottom" << std::endl;
                FloorContacts.insert(collision_e->With);
                CanJump = true;
            }
        }

        void CharacterController::OnCollisionEnded(const Event* e) {
            const CollisionEndedEvent* collision_e = reinterpret_cast<const CollisionEndedEvent*>(e);
                //std::cout << "Ended collision" << std::endl;
            FloorContacts.erase(collision_e->With);
            if(FloorContacts.empty()) {
                CanJump = false;
            }
        }

//        void CharacterController::OnCollision(const Event* e) {
//            const CollisionEvent* collision_e = reinterpret_cast<const CollisionEvent*>(e);
//
//            std::cout << "Collision" << std::endl;
//
//            if(collision_e->Side == CollisionSide::Top) {
//                CanJump = true;
//            }
//        }

        void CharacterController::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer controller = root.GetChild("CharacterController");
            controller.WriteInt("RightKey", RightKey);
            controller.WriteInt("LeftKey", LeftKey);
            controller.WriteInt("JumpKey", JumpKey);
            controller.WriteFloat("Acceleration", Acceleration);
            controller.WriteFloat("JumpSpeed", JumpSpeed);
            controller.WriteFloat("Drag", Drag);
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
            controller.ReadFloat("Acceleration", Acceleration);
            controller.ReadFloat("JumpSpeed", JumpSpeed);
            controller.ReadFloat("Drag", Drag);
            controller.ReadString("WalkSound", WalkSound);
            controller.ReadString("JumpSound", JumpSound);
            controller.ReadString("LandSound", LandSound);
            controller.ReadString("WalkAnimation", WalkAnimation);
            controller.ReadString("JumpAnimation", JumpAnimation);
        }
    }
}
