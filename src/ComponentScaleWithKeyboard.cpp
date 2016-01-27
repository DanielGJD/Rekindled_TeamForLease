/*!
    \file   ComponentScaleWithKeyboard.cpp
    \author Christopher Hudson

    \brief
        Defines a component to allow scaling an object with the keyboard

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentScaleWithKeyboard.h"
#include "Keys.h"
#include "Engine.h"
#include "KeyboardEvent.h"
#include "ComponentTransform.h"
#include "SoundEmitter.h"
#include "Entity.h"

namespace ForLeaseEngine {
    namespace Components {
        ScaleWithKeyboard::ScaleWithKeyboard(Entity& owner)
                                            : Component(owner, ComponentType::Transform), Active(false), ScaleSpeed(1),
                                              ScaleXUpKey(Keys::D), ScaleXDownKey(Keys::A),
                                              ScaleYUpKey(Keys::W), ScaleYDownKey(Keys::S),
                                              MaxXScale(2), MinXScale(0.5f), MaxYScale(2), MinYScale(0.5f),
                                              ScaleXUp(false), ScaleXDown(false),
                                              ScaleYUp(false), ScaleYDown(false), ScaleSound("") {}

        ScaleWithKeyboard::~ScaleWithKeyboard() {
            ForLease->Dispatcher.Detach(this, "KeyDown");
            ForLease->Dispatcher.Detach(this, "KeyUp");
        }

        ScaleWithKeyboard* ScaleWithKeyboard::Create(Entity& owner) {
            ScaleWithKeyboard* component = new ScaleWithKeyboard(owner);
            component->Initialize();
            return component;
        }

        ComponentType ScaleWithKeyboard::GetType() {
            return Type;
        }

        void ScaleWithKeyboard::Update() {
            if(Active) {
                Components::Transform* trans = Parent.GetComponent<Components::Transform>();
                float dt = ForLease->FrameRateController().GetUnscaledDt();
                if(ScaleXUp) {
                    trans->ScaleX += dt * ScaleSpeed;
                }
                if(ScaleXDown) {
                    trans->ScaleX -= dt * ScaleSpeed;
                }
                if(ScaleYUp) {
                    trans->ScaleY += dt * ScaleSpeed;
                }
                if(ScaleYDown) {
                    trans->ScaleY -= dt * ScaleSpeed;
                }

                if(trans->ScaleX > MaxXScale) {
                    trans->ScaleX = MaxXScale;
                }
                if(trans->ScaleX < MinXScale) {
                    trans->ScaleX = MinXScale;
                }
                if(trans->ScaleY > MaxYScale) {
                    trans->ScaleY = MaxYScale;
                }
                if(trans->ScaleY < MinYScale) {
                    trans->ScaleY = MinYScale;
                }
            }
        }

        void ScaleWithKeyboard::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned int>(Type));
            Serializer scaler = root.GetChild("ScaleWithKeyboard");
            scaler.WriteUint("Type", static_cast<unsigned int>(Type));
            scaler.WriteBool("Active", Active);
            scaler.WriteFloat("ScaleSpeed", ScaleSpeed);
            scaler.WriteInt("ScaleXUpKey", ScaleXUpKey);
            scaler.WriteInt("ScaleXDownKey", ScaleXDownKey);
            scaler.WriteInt("ScaleYUpKey", ScaleYUpKey);
            scaler.WriteInt("ScaleYDownKey", ScaleYDownKey);
            scaler.WriteFloat("MaxXScale", MaxXScale);
            scaler.WriteFloat("MinXScale", MinXScale);
            scaler.WriteFloat("MaxYScale", MaxYScale);
            scaler.WriteFloat("MinYScale", MinYScale);
            scaler.WriteString("ScaleSound", ScaleSound);
            root.Append(scaler, "ScaleWithKeyboard");
        }

        void ScaleWithKeyboard::Deserialize(Serializer& root) {
            Serializer scaler = root.GetChild("ScaleWithKeyboard");
            scaler.ReadBool("Active", Active);
            scaler.ReadFloat("ScaleSpeed", ScaleSpeed);
            scaler.ReadInt("ScaleXUpKey", ScaleXUpKey);
            scaler.ReadInt("ScaleXDownKey", ScaleXDownKey);
            scaler.ReadInt("ScaleYUpKey", ScaleYUpKey);
            scaler.ReadInt("ScaleYDownKey", ScaleYDownKey);
            scaler.ReadFloat("MaxXScale", MaxXScale);
            scaler.ReadFloat("MinXScale", MinXScale);
            scaler.ReadFloat("MaxYScale", MaxYScale);
            scaler.ReadFloat("MinYScale", MinYScale);
            scaler.ReadString("ScaleSound", ScaleSound);
        }

        void ScaleWithKeyboard::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "KeyDown", &ScaleWithKeyboard::OnKeyDown);
            ForLease->Dispatcher.Attach(NULL, this, "KeyUp", &ScaleWithKeyboard::OnKeyUp);

        }

        void ScaleWithKeyboard::OnKeyDown(Event const* e) {
            KeyboardEvent const* key_e = static_cast<KeyboardEvent const*>(e);

            SoundEmitter* emitter = Parent.GetComponent<SoundEmitter>();
            bool canPlaySound = false;
            if(emitter && !ScaleXUp && !ScaleXDown && !ScaleYUp && !ScaleYDown && Active) {
                canPlaySound = true;
            }
            if(key_e->Key == ScaleXUpKey)
                ScaleXUp = true;
            if(key_e->Key == ScaleXDownKey)
                ScaleXDown = true;
            if(key_e->Key == ScaleYUpKey)
                ScaleYUp = true;
            if(key_e->Key == ScaleYDownKey)
                ScaleYDown = true;

            if(canPlaySound && (ScaleXUp || ScaleXDown || ScaleYUp || ScaleYDown ))
                emitter->PlayEvent(ScaleSound);
        }

        void ScaleWithKeyboard::OnKeyUp(Event const* e) {
            KeyboardEvent const* key_e = static_cast<KeyboardEvent const*>(e);
            if(key_e->Key == ScaleXUpKey)
                ScaleXUp = false;
            if(key_e->Key == ScaleXDownKey)
                ScaleXDown = false;
            if(key_e->Key == ScaleYUpKey)
                ScaleYUp = false;
            if(key_e->Key == ScaleYDownKey)
                ScaleYDown = false;

            SoundEmitter* emitter = Parent.GetComponent<SoundEmitter>();
            if(emitter && !ScaleXUp && !ScaleXDown && !ScaleYUp && !ScaleYDown && Active) {
                emitter->StopEvent(ScaleSound);
            }
        }
    }
}
