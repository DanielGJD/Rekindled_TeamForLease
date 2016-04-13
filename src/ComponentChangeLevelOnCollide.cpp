/*!
    \file   ComponentChangeLevelOnCollide.cpp
    \author Christopher Hudson

    \brief
        Defines a component to change the level upon collision with an entity

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentChangeLevelOnCollide.h"
#include "Engine.h"
#include "State.h"
#include "LevelComponentRenderer.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentSoundEmitter.h"
#include "CollisionEvent.h"

namespace ForLeaseEngine {
    namespace Components {
        ChangeLevelOnCollide::ChangeLevelOnCollide(Entity& parent, bool active,
                                                   std::string levelName, std::string triggerObjectName, std::string triggerSoundName,
                                                   float onSwitchTimeScale, float fadeOutTime)
                                                  : Component(parent, ComponentType::Transform | ComponentType::Collision),
                                                    Active(active), LevelName(levelName), TriggerObjectName(triggerObjectName), TriggerSoundName(triggerSoundName),
                                                    OnSwitchTimeScale(onSwitchTimeScale), FadeOutTime(fadeOutTime),
                                                    Switching(false), Timer(0) {}

        ChangeLevelOnCollide::~ChangeLevelOnCollide() {
            ForLease->Dispatcher.Detach(this, "CollisionStarted");
        }

        ComponentType ChangeLevelOnCollide::GetType() { return Type; }

        void ChangeLevelOnCollide::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "CollisionStarted", &ChangeLevelOnCollide::CollisionStarted, &Parent);
        }

        void ChangeLevelOnCollide::Update() {
            if(!Active)
                return;
            if(Switching) {
                Timer += ForLease->FrameRateController().GetUnscaledDt();
                if(Timer >= FadeOutTime) {
                    ForLease->GameStateManager().SetState(LevelName);
                    return;
                }
                LevelComponents::Renderer* render = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();
                render->SetOverlayColor(0, 0, 0, Timer / FadeOutTime);
            }
//            else {
//                Collision* collider = Parent.GetComponent<Collision>();
//                if(collider->CollidedWith(TriggerObjectName)) {
//                    Switching = true;
//                    SoundEmitter* emitter = Parent.GetComponent<SoundEmitter>();
//                    if(TriggerSoundName.compare("") && emitter) {
//                        emitter->SetVolume(1.0f,TriggerSoundName);
//                        emitter->StopEvent(TriggerSoundName);
//                        emitter->PlayEvent(TriggerSoundName);
//                    }
//                    ForLease->FrameRateController().TimeScaling(OnSwitchTimeScale);
//                }
//            }
        }

        void ChangeLevelOnCollide::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned int>(Type));
            Serializer changeLevel = root.GetChild("ChangeLevelOnCollide");
            changeLevel.WriteUint("Type", static_cast<unsigned int>(Type));
            changeLevel.WriteBool("Active", Active);
            changeLevel.WriteString("LevelName", LevelName);
            changeLevel.WriteString("TriggerObjectName", TriggerObjectName);
            changeLevel.WriteString("TriggerSoundName", TriggerSoundName);
            changeLevel.WriteFloat("OnSwitchTimeScale", OnSwitchTimeScale);
            changeLevel.WriteFloat("FadeOutTime", FadeOutTime);
            root.Append(changeLevel, "ChangeLevelOnCollide");
        }

        void ChangeLevelOnCollide::Deserialize(Serializer& root) {
            Serializer changeLevel = root.GetChild("ChangeLevelOnCollide");
            changeLevel.ReadBool("Active", Active);
            changeLevel.ReadString("LevelName", LevelName);
            changeLevel.ReadString("TriggerObjectName", TriggerObjectName);
            changeLevel.ReadString("TriggerSoundName", TriggerSoundName);
            changeLevel.ReadFloat("OnSwitchTimeScale", OnSwitchTimeScale);
            changeLevel.ReadFloat("FadeOutTime", FadeOutTime);
        }

        void ChangeLevelOnCollide::CollisionStarted(const Event* e) {
            const CollisionEvent* collision_e = reinterpret_cast<const CollisionEvent*>(e);
            std::cout << "COLLISION STARTED ON CHANGE LEVEL" << std::endl;

            if(collision_e->With->GetName().compare(TriggerObjectName) == 0) {
                Switching = true;
                //ForLease->sound->PlayEvent(TriggerSoundName);
                ForLease->FrameRateController().TimeScaling(OnSwitchTimeScale);
            }
        }
    }
}
