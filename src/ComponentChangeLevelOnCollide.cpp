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

namespace ForLeaseEngine {
    namespace Components {
        ChangeLevelOnCollide::ChangeLevelOnCollide(Entity& parent, bool active,
                                                   std::string levelName, std::string triggerObjectName, std::string triggerSoundName,
                                                   float onSwitchTimeScale, float fadeOutTime)
                                                  : Component(parent, ComponentType::Transform | ComponentType::Collision),
                                                    Active(active), LevelName(levelName), TriggerObjectName(triggerObjectName), TriggerSoundName(triggerSoundName),
                                                    OnSwitchTimeScale(onSwitchTimeScale), FadeOutTime(fadeOutTime),
                                                    Switching(false), Timer(0) {}

        ChangeLevelOnCollide::~ChangeLevelOnCollide() {}

        ComponentType ChangeLevelOnCollide::GetType() { return Type; }

        void ChangeLevelOnCollide::Update() {
            if(Switching) {
                Timer += ForLease->FrameRateController().GetUnscaledDt();
                if(Timer >= FadeOutTime) {
                    ForLease->GameStateManager().SetState(LevelName);
                    return;
                }
                LevelComponents::Renderer* render = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();
                render->SetOverlayColor(0, 0, 0, Timer / FadeOutTime);
            }
            else {
                Collision* collider = Parent.GetComponent<Collision>();
                if(collider->CollidedWith && collider->CollidedWith->GetName().compare(TriggerObjectName) == 0) {
                    Switching = true;
                    SoundEmitter* emitter = Parent.GetComponent<SoundEmitter>();
                    if(emitter) {
                        emitter->Play(TriggerSoundName);
                    }
                    ForLease->FrameRateController().TimeScaling(OnSwitchTimeScale);
                }
            }
        }

        void ChangeLevelOnCollide::Serialize(Serializer& root) {
        }

        void ChangeLevelOnCollide::Deserialize(Serializer& root) {
        }
    }
}
