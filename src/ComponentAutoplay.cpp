/*!
    \file   ComponentAutoplay.cpp
    \author Christopher Hudson

    \brief
        Defines a class to dispatch input to the character controller

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentAutoplay.h"
#include "Engine.h"
#include "KeyboardEvent.h"
#include "ComponentCharacterController.h"
#include "Keys.h"
#include "Random.h"
#include "Entity.h"
#include "Component.h"

namespace ForLeaseEngine {
    namespace Components {
        Autoplay::Autoplay(Entity& parent, bool active, float leftToggleTime,
                           float rightToggleTime, float jumpTime, float randomTime)
                           : Component(parent, ComponentType::PlayerController), Active(active), LeftToggleTime(leftToggleTime),
                             RightToggleTime(rightToggleTime), JumpTime(jumpTime), RandomTime(randomTime),
                             LeftTimer(0), RightTimer(0), JumpTimer(0), LeftDown(false), RightDown(false) {}

        Autoplay::~Autoplay() {}

        ComponentType Autoplay::GetType() { return Type; }

        void Autoplay::Update() {
            float dt = ForLease->FrameRateController().GetDt();
            Components::CharacterController* cont = Parent.GetComponent<Components::CharacterController>();

            LeftTimer += dt;
            RightTimer += dt;
            JumpTimer += dt;

            if(LeftTimer >= LeftToggleTime) {
                LeftDown = !LeftDown;

                if(LeftDown) {
                    KeyboardEvent e = KeyboardEvent("KeyDown");
                    e.Key = cont->LeftKey;
                    e.State = KeyState::Pressed;
                    ForLease->Dispatcher.Dispatch(&e, this);
                }
                else {
                    KeyboardEvent e = KeyboardEvent("KeyUp");
                    e.Key = cont->LeftKey;
                    e.State = KeyState::Released;
                    ForLease->Dispatcher.Dispatch(&e, this);
                }

                LeftTimer = RandomFloat(-RandomTime, RandomTime);
            }

            if(RightTimer >= RightToggleTime) {
                LeftDown = !LeftDown;

                if(LeftDown) {
                    KeyboardEvent e = KeyboardEvent("KeyDown");
                    e.Key = cont->RightKey;
                    e.State = KeyState::Pressed;
                    ForLease->Dispatcher.Dispatch(&e, this);
                }
                else {
                    KeyboardEvent e = KeyboardEvent("KeyUp");
                    e.Key = cont->RightKey;
                    e.State = KeyState::Released;
                    ForLease->Dispatcher.Dispatch(&e, this);
                }

                RightTimer = RandomFloat(-RandomTime, RandomTime);
            }

            if(JumpTimer >= JumpTime) {
                {
                    KeyboardEvent e = KeyboardEvent("KeyDown");
                    e.Key = cont->JumpKey;
                    e.State = KeyState::Pressed;
                    ForLease->Dispatcher.Dispatch(&e, this);
                }
                {
                    KeyboardEvent e = KeyboardEvent("KeyUp");
                    e.Key = cont->JumpKey;
                    e.State = KeyState::Released;
                    ForLease->Dispatcher.Dispatch(&e, this);
                }

                JumpTimer = RandomFloat(-RandomTime, RandomTime);
            }
        }

        void Autoplay::Serialize(Serializer& root) {
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            Serializer autoplay = root.GetChild("Autoplay");
            autoplay.WriteBool("Active", Active);
            autoplay.WriteFloat("LeftToggleTime", LeftToggleTime);
            autoplay.WriteFloat("RightToggleTime", RightToggleTime);
            autoplay.WriteFloat("JumpTime", JumpTime);
            autoplay.WriteFloat("RandomTime", RandomTime);
            root.Append(autoplay, "Autoplay");
        }

        void Autoplay::Deserialize(Serializer& root) {
            Serializer autoplay = root.GetChild("Autoplay");
            autoplay.ReadBool("Active", Active);
            autoplay.ReadFloat("LeftToggleTime", LeftToggleTime);
            autoplay.ReadFloat("RightToggleTime", RightToggleTime);
            autoplay.ReadFloat("JumpTime", JumpTime);
            autoplay.ReadFloat("RandomTime", RandomTime);
        }
    }
}
