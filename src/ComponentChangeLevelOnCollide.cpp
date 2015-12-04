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

namespace ForLeaseEngine {
    namespace Components {
        ChangeLevelOnCollide::ChangeLevelOnCollide(Entity& parent, bool active,
                                                   std::string levelName, std::string triggerObjectName,
                                                   float onSwitchTimeScale, float fadeOutTime)
                                                  : Component(parent, ComponentType::Transform | ComponentType::Collision),
                                                    Active(active), LevelName(levelName), TriggerObjectName(triggerObjectName),
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
                //Entity* cameraEntity = ForLease->GameStateManager().CurrentState().GetEntityByID(render->GetCameraID());
                //if(cameraEntity) {
                    //Transform* cameraTrans = cameraEntity->GetComponent<Transform>();
                    //Camera* camera = cameraEntity->GetComponent<Camera>();
                    //float height = camera->Size;
                    //float width = static_cast<float>(ForLease->GameWindow->GetXResolution()) / ForLease->GameWindow->GetYResolution() * camera->Size;
                    //render->DrawRectangleFilled(cameraTrans->Position, width, height, 0, BlendMode::ALPHA);
                //}
            }
            else {
                Collision* collider = Parent.GetComponent<Collision>();
                if(collider->CollidedWith && collider->CollidedWith->GetName().compare(TriggerObjectName) == 0) {
                    Switching = true;
                }
            }
        }

        void ChangeLevelOnCollide::Serialize(Serializer& root) {
        }

        void ChangeLevelOnCollide::Deserialize(Serializer& root) {
        }
    }
}
