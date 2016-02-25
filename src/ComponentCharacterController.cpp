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

namespace ForLeaseEngine {
    namespace Components {
        CharacterController::CharacterController(Entity& owner)
                                                : Component(owner, ComponentType::Physics | ComponentType::Collision),
                                                  RightKey(Keys::D), LeftKey(Keys::A), JumpKey(Keys::W),
                                                  MoveSpeed(0.0f), JumpSpeed(0),Drag(0), maxSpeed(0), Friction(0),WalkSound(""), JumpSound(""), LandSound(""),
                                                  WalkAnimation(""), JumpAnimation(""), CanJump(false) , CheckMove(false){};

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
//            std::cout << "Animation: " << Parent.GetComponent<Components::Model>()->GetAnimation() << std::endl;
            //std::cout << "Walk ani: " << WalkAnimation << std::endl;
            Collision* collider = Parent.GetComponent<Collision>();
            bool couldJump = CanJump;
            CanJump = false;
            if(collider->CollidedLastFrame && collider->CollidedWithSide == Components::Collision::Side::Top)
                CanJump = true;

            if(!couldJump && CanJump) {
                SoundEmitter* emitter = Parent.GetComponent<SoundEmitter>();
                if(emitter)
                    emitter->PlayEvent(JumpSound);
            }


            float Drag = .05;
            float maxSpeed = 1.5;
            float Friction = 0.5;
            if(!CheckMove)
            {
                Physics* rbody = Parent.GetComponent<Physics>();
                Vector currentVelocity = rbody->Velocity;
                currentVelocity.Rotate(currentVelocity, 180);
                currentVelocity = currentVelocity * Drag;
                rbody->Velocity = (rbody->Velocity - currentVelocity);

                if(rbody->Velocity[0] > maxSpeed)
                {
                    printf("updateV %f\n", rbody->Velocity[0]);
                    rbody->Velocity[0] = rbody->Velocity[0] - Friction;
                    if(rbody->Velocity[0] < maxSpeed)
                        rbody->Velocity[0]  = rbody->Velocity[0] + Friction;
                }
                if(rbody->Velocity[0] < -maxSpeed)
                {
                    printf("negativeV %f\n", rbody->Velocity[0]);
                    rbody->Velocity[0] = rbody->Velocity[0] + Friction;
                    if(rbody->Velocity[0] > -maxSpeed)
                        rbody->Velocity[0] = rbody->Velocity[0] - Friction;
                }
            }
            Model* model = Parent.GetComponent<Model>();
            if(model && model->AnimationActive && model->GetAnimation().compare(WalkAnimation) == 0) {
                unsigned int currentFrame = model->GetFrame();
                if(currentFrame != LastAnimationFrame && (currentFrame == 1 || currentFrame == 5)) {
                    SoundEmitter* emitter = Parent.GetComponent<SoundEmitter>();
                    if(emitter)
                        emitter->PlayEvent(WalkSound);
                }
            }
        };

        void CharacterController::OnKeyDown(const Event* e) {
            const KeyboardEvent* key_e = static_cast<const KeyboardEvent*>(e);
            Components::SoundEmitter* emitter = Parent.GetComponent<Components::SoundEmitter>();
            Components::Collision* collider = Parent.GetComponent<Components::Collision>();
            Components::Model* model = Parent.GetComponent<Components::Model>();
            Physics* rbody = Parent.GetComponent<Physics>();
            if(key_e->Key == LeftKey) {

                //rbody->Velocity += Vector(-MoveSpeed, 0);
                //rbody->Acceleration += Vector(-MoveSpeed, 0 ) * 5;
                if(rbody->Acceleration.GetX() < MoveSpeed)
                    rbody->Acceleration += Vector(-MoveSpeed * 1000 * ForLease->FrameRateController().GetDt(), 0 );
                    //printf("LEFTa %f", rbody->Acceleration[0]);
                   // printf("pressRV %f", rbody->Velocity[0]);
                    CheckMove = true;


                if(model)
                    model->FlipY = true;
                if(collider->CollidedLastFrame && collider->CollidedWithSide == Collision::Side::Top) {
                    if (emitter) {
                        //emitter->Looping = true;
                        emitter->SetVolume(1.0f, WalkSound);
                        emitter->StopEvent(WalkSound);
                        emitter->PlayEvent(WalkSound);
                    }

                    if(model)
                        model->SetAnimation(WalkAnimation);
                }
            }
            else if(key_e->Key == RightKey) {
                //Physics* rbody = Parent.GetComponent<Physics>();
                if(rbody->Acceleration.GetX() > -MoveSpeed)
               //rbody->Velocity += Vector(MoveSpeed, 0);
                    rbody->Acceleration += Vector(MoveSpeed * 1000 * ForLease->FrameRateController().GetDt(), 0) ;
                    //printf("RIGHTa %f", rbody->Acceleration[0]);
                    printf("pressLV %f\n", rbody->Velocity[0]);

                    CheckMove = true;

                if(model)
                    model->FlipY = false;
                if(collider->CollidedLastFrame && collider->CollidedWithSide == Collision::Side::Top) {
                    if (emitter) {
                        emitter->SetVolume(1.0f, WalkSound);
                        emitter->StopEvent(WalkSound);
                        emitter->PlayEvent(WalkSound);
                    }
                    if(model)
                        model->SetAnimation(WalkAnimation);
                }
            }
            else if(key_e->Key == JumpKey) {
                Physics* rbody = Parent.GetComponent<Physics>();
               // Collision* collider = Parent.GetComponent<Collision>();
                if(CanJump) {
                    rbody->Velocity += Vector(0, JumpSpeed);
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
                Physics* rbody = Parent.GetComponent<Physics>();

                //rbody->Acceleration += Vector(MoveSpeed * 900 * ForLease->FrameRateController().GetDt(), 0) ;
//                while(rbody->Velocity[0] < 0){
//                    rbody->Velocity[0] = rbody->Velocity[0] + deccelerate;
//                    if(rbody->Velocity[0] > 0)
//                        rbody->Velocity[0] = 0;
//                }
                CheckMove = false;
                //if(rbody->Acceleration[0] == 0)
                rbody->Acceleration[0] = 0;
                //rbody->Acceleration += Vector(-MoveSpeed * 20, 0 );

                //rbody
                printf("releaseLV %f\n", rbody->Velocity[0]);
                //printf("rightA %f", rbody->Acceleration[0]);

                //rbody->Acceleration += Vector(MoveSpeed * 20, 0 );
                //rbody->Velocity[0] = 0;
//                if((rbody->Acceleration.GetX()) > MoveSpeed)
//                    rbody->Velocity.SetDecrementX(MoveSpeed);
//                if((rbody->Acceleration.GetX()) < -MoveSpeed)
//                   rbody->Velocity.SetIncrementX(MoveSpeed);
//                if((rbody->Velocity.GetX()) > MoveSpeed)
//                    rbody->Velocity.SetDecrementX(MoveSpeed);
//                if((rbody->Velocity.GetX()) < -MoveSpeed)
//                   rbody->Velocity.SetIncrementX(MoveSpeed);
                if(emitter)
                    emitter->SetPause(true, WalkSound);

                if(model)
                    model->SetAnimation("");
            }
            else if(key_e->Key == RightKey) {
                Physics* rbody = Parent.GetComponent<Physics>();
                //rbody->Acceleration += Vector(-MoveSpeed * 900 * ForLease->FrameRateController().GetDt(), 0 );
//                rbody->Velocity[0] = rbody->Velocity[0]  - deccelerate;
//                if(rbody->Acceleration[0] == 0)
//                    rbody->Acceleration[0] = 0;
//
//                while(rbody->Velocity[0] > 0){
//                    rbody->Velocity[0] = rbody->Velocity[0] - deccelerate;
//                    if(rbody->Velocity[0] < 0)
//                        rbody->Velocity[0] = 0;
//                }
                //rbody->Acceleration += Vector(-MoveSpeed * 20, 0 );
                //if(rbody->Velocity[0] = 0)
                    //rbody->Velocity[0] = 0;
                    //rbody->Velocity[0] = 0;
                rbody->Acceleration[0] = 0;
                CheckMove = false;
                printf("releaseRV %f\n", rbody->Velocity[0]);
                //printf("rightA %f", rbody->Acceleration[0]);
                //rbody->Acceleration += Vector(-MoveSpeed * 20, 0 );

                //rbody->Velocity[0] = 0;
                //rbody->Velocity[0] = 0;

//                if((rbody->Acceleration.GetX()) > MoveSpeed)
//                    rbody->Velocity.SetDecrementX(MoveSpeed * 20);
//                if((rbody->Acceleration.GetX()) < -MoveSpeed)
//                   rbody->Velocity.SetIncrementX(MoveSpeed * 20);
//                if((rbody->Velocity.GetX()) > MoveSpeed)
//                    rbody->Velocity.SetDecrementX(MoveSpeed);
//                if((rbody->Velocity.GetX()) < -MoveSpeed)
//                   rbody->Velocity.SetIncrementX(MoveSpeed);
                if(emitter)
                    emitter->SetPause(true, WalkSound);
                if(model)
                    model->SetAnimation("");
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
            controller.WriteFloat("Drag", Drag);
            controller.WriteFloat("maxSpeed", maxSpeed);
            controller.WriteFloat("Friction", Friction);
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
            controller.ReadFloat("Drag", Drag);
            controller.ReadFloat("maxSpeed", maxSpeed);
            controller.ReadFloat("Friction", Friction);
            controller.ReadString("WalkSound", WalkSound);
            controller.ReadString("JumpSound", JumpSound);
            controller.ReadString("LandSound", LandSound);
            controller.ReadString("WalkAnimation", WalkAnimation);
            controller.ReadString("JumpAnimation", JumpAnimation);
        }
    }
}
