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
#include "Interpolation.h"

namespace ForLeaseEngine {
    namespace Components {
        ScaleWithKeyboard::ScaleWithKeyboard(Entity& owner, bool active, float scaleSpeed,
                                             int scaleUpKey, int scaleDownKey, float maxXScale, float maxYScale, float scale, std::string scaleSound)
                                            : Component(owner, ComponentType::Transform), Active(active), ScaleSpeed(scaleSpeed),
                                              ScaleUpKey(scaleUpKey), ScaleDownKey(scaleDownKey),
                                              MaxXScale(maxXScale), MaxYScale(maxYScale), Scale(scale), ScaleSound(scaleSound),
                                              ScaleUp(false), ScaleDown(false), NaturalWidth(1), NaturalHeight(1) {}

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
                if(ScaleUp) {
                    Scale += ScaleSpeed * dt;
                }
                if(ScaleDown) {
                    Scale -= ScaleSpeed * dt;
                }
                if(Scale < 0) {
                    Scale = 0;
                }
                else if(Scale > 1) {
                    Scale = 1;
                }

                trans->ScaleX = Interpolation::Linear(NaturalWidth, NaturalWidth * MaxXScale, Scale);
                trans->ScaleY = Interpolation::Linear(NaturalHeight, NaturalHeight * MaxYScale, Scale);
            }
        }

        void ScaleWithKeyboard::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned int>(Type));
            Serializer scaler = root.GetChild("ScaleWithKeyboard");
            scaler.WriteUint("Type", static_cast<unsigned int>(Type));
            scaler.WriteBool("Active", Active);
            scaler.WriteFloat("ScaleSpeed", ScaleSpeed);
            scaler.WriteInt("ScaleUpKey", ScaleUpKey);
            scaler.WriteInt("ScaleDownKey", ScaleDownKey);
            scaler.WriteFloat("MaxXScale", MaxXScale);
            scaler.WriteFloat("MaxYScale", MaxYScale);
            scaler.WriteFloat("Scale", Scale);
            scaler.WriteString("ScaleSound", ScaleSound);
            root.Append(scaler, "ScaleWithKeyboard");
        }

        void ScaleWithKeyboard::Deserialize(Serializer& root) {
            Serializer scaler = root.GetChild("ScaleWithKeyboard");
            scaler.ReadBool("Active", Active);
            scaler.ReadFloat("ScaleSpeed", ScaleSpeed);
            scaler.ReadInt("ScaleUpKey", ScaleUpKey);
            scaler.ReadInt("ScaleDownKey", ScaleDownKey);
            scaler.ReadFloat("MaxXScale", MaxXScale);
            scaler.ReadFloat("MaxYScale", MaxYScale);
            scaler.ReadFloat("Scale", Scale);
            scaler.ReadString("ScaleSound", ScaleSound);
        }

        void ScaleWithKeyboard::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "KeyDown", &ScaleWithKeyboard::OnKeyDown);
            ForLease->Dispatcher.Attach(NULL, this, "KeyUp", &ScaleWithKeyboard::OnKeyUp);
            Transform* trans = Parent.GetComponent<Components::Transform>();
            NaturalWidth = trans->ScaleX;
            NaturalHeight = trans->ScaleY;
        }

        void ScaleWithKeyboard::OnKeyDown(Event const* e) {
            KeyboardEvent const* key_e = static_cast<KeyboardEvent const*>(e);

            SoundEmitter* emitter = Parent.GetComponent<SoundEmitter>();
            bool canPlaySound = false;
            if(emitter && !ScaleUp && !ScaleDown) {
                canPlaySound = true;
            }
            if(key_e->Key == ScaleUpKey)
                ScaleUp = true;
            if(key_e->Key == ScaleDownKey)
                ScaleDown = true;


            if(canPlaySound && (ScaleUp || ScaleDown))
                emitter->PlayEvent(ScaleSound);
        }

        void ScaleWithKeyboard::OnKeyUp(Event const* e) {
            KeyboardEvent const* key_e = static_cast<KeyboardEvent const*>(e);
            if(key_e->Key == ScaleUpKey)
                ScaleUp = false;
            if(key_e->Key == ScaleDownKey)
                ScaleDown = false;

            SoundEmitter* emitter = Parent.GetComponent<SoundEmitter>();
            if(emitter && !ScaleUp && !ScaleDown && Active) {
                emitter->StopEvent(ScaleSound);
            }
        }
    }
}
