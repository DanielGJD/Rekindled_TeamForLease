/*!
    \file   ComponentFadeWithDistance.cpp
    \author Christopher Hudson

    \brief
        Defines a component that makes an entity fade out with a larger distance to another entity

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/
#include "ComponentFadeWithDistance.h"
#include "ComponentTransform.h"
#include "ComponentSprite.h"
#include "Vector.h"
#include "State.h"
#include "Engine.h"
#include "Entity.h"
#include <cmath>

namespace ForLeaseEngine {
    namespace Components {
        FadeWithDistance::FadeWithDistance(Entity& owner, bool active, bool xdirection, bool ydirection,
                                           float fadeBeginDistance, float fadeEndDistance, unsigned long trackedEntityID)
                                          : Component(owner, ComponentType::Transform),
                                            Active(active), XDirection(xdirection), YDirection(ydirection),
                                            FadeBeginDistance(fadeBeginDistance), FadeEndDistance(fadeEndDistance),
                                            TrackedEntityID(trackedEntityID) {}

        FadeWithDistance::~FadeWithDistance() {}

        ComponentType FadeWithDistance::GetType() { return Type; }

        void FadeWithDistance::Update() {
            if(!Active)
                return;
            Entity* trackedEntity = ForLease->GameStateManager().CurrentState().GetEntityByID(TrackedEntityID);
            if(trackedEntity) {
                Components::Transform* trackedTrans = trackedEntity->GetComponent<Components::Transform>();
                Components::Transform* myTrans = Parent.GetComponent<Components::Transform>();
                float distance;

                if(XDirection && YDirection) {
                    distance = Point::Distance(trackedTrans->Position, myTrans->Position);
                }
                else if(XDirection) {
                    distance = trackedTrans->Position[0] - myTrans->Position[0];
                    if(distance < 0)
                        distance *= -1;
                }
                else if(YDirection) {
                    distance = trackedTrans->Position[1] - myTrans->Position[1];
                    if(distance < 0)
                        distance *= -1;
                }
                else {
                    return;
                }

                Components::Sprite* mySprite = Parent.GetComponent<Components::Sprite>();
                Components::SpriteText* mySpriteText = Parent.GetComponent<Components::SpriteText>();

                if (mySprite) {
                    float alpha;
                    if (distance < FadeBeginDistance)
                        alpha = 1;
                    else if (distance > FadeEndDistance)
                        alpha = 0;
                    else
                        alpha = 1 - (distance - FadeBeginDistance) / (FadeEndDistance - FadeBeginDistance);

                    mySprite->SpriteColor.SetA(alpha);
                }
                if (mySpriteText) {
                    float alpha;
                    if (distance < FadeBeginDistance)
                        alpha = 1;
                    else if (distance > FadeEndDistance)
                        alpha = 0;
                    else
                        alpha = 1 - (distance - FadeBeginDistance) / (FadeEndDistance - FadeBeginDistance);

                    mySpriteText->TextColor.SetA(alpha);
                }
            }
        }

        void FadeWithDistance::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned int>(Type));
            Serializer fadeWithDistance = root.GetChild("FadeWithDistance");
            fadeWithDistance.WriteUint("Type", static_cast<unsigned int>(Type));
            fadeWithDistance.WriteBool("Active", Active);
            fadeWithDistance.WriteBool("XDirection", XDirection);
            fadeWithDistance.WriteBool("YDirection", YDirection);
            fadeWithDistance.WriteFloat("FadeBeginDistance", FadeBeginDistance);
            fadeWithDistance.WriteFloat("FadeEndDistance", FadeEndDistance);
            fadeWithDistance.WriteUint("TrackedEntityID", TrackedEntityID);
            root.Append(fadeWithDistance, "FadeWithDistance");
        }

        void FadeWithDistance::Deserialize(Serializer& root) {
            Serializer fadeWithDistance = root.GetChild("FadeWithDistance");
            fadeWithDistance.ReadBool("Active", Active);
            fadeWithDistance.ReadBool("XDirection", XDirection);
            fadeWithDistance.ReadBool("YDirection", YDirection);
            fadeWithDistance.ReadFloat("FadeBeginDistance", FadeBeginDistance);
            fadeWithDistance.ReadFloat("FadeEndDistance", FadeEndDistance);
            unsigned int id;
            fadeWithDistance.ReadUint("TrackedEntityID", id);
            TrackedEntityID = id;
        }
    }
}
