/*!
    \file   ComponentFollow.cpp
    \author Christopher Hudson

    \brief
        Defines a component to make an entity follow another entity

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentFollow.h"
#include "State.h"
#include "Engine.h"
#include "Interpolation.h"
#include "ComponentTransform.h"

namespace ForLeaseEngine {
    namespace Components {
        Follow::Follow(Entity& parent, bool active,
                                         float followBeginDistance, float followEndDistance,
                                         unsigned long followEntityID, Vector const& offset)
                                        : Component(parent, ComponentType::Transform),
                                          Active(active), FollowBeginDistance(followBeginDistance),
                                          FollowEndDistance(followEndDistance), FollowEntityID(followEntityID), Offset(offset) {}


        Follow::~Follow() {}

        ComponentType Follow::GetType() { return Type; }

        void Follow::Update() {
            if(!Active)
                return;
            Entity* entity = ForLease->GameStateManager().CurrentState().GetEntityByID(FollowEntityID);
            if(entity) {
                Transform* followTrans = entity->GetComponent<Components::Transform>();
                Transform* myTrans = Parent.GetComponent<Components::Transform>();
//                float distance = Point::Distance(followTrans->Position, myTrans->Position);
//                float t;
//                if(distance < FollowBeginDistance)
//                    t = 0;
//                else if(distance > FollowEndDistance)
//                    t = 1;
//                else
//                    t = (distance - FollowBeginDistance) / (FollowEndDistance - FollowBeginDistance);
//
//                Vector fullMovement = followTrans->Position - myTrans->Position;
//                float moveScale = Interpolation::PowerIn(2, 0, 1, t);
//                Vector scaledMovement = Vector::Scale(fullMovement, moveScale * ForLease->FrameRateController().GetDt());
//                myTrans->Position += scaledMovement;
                Vector direction = followTrans->Position - myTrans->Position + Offset;
                myTrans->Position += direction * ForLease->FrameRateController().GetDt();
            }
        }

        void Follow::Serialize(Serializer& root) {
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            Serializer follow = root.GetChild("Follow");
            follow.WriteBool("Active", Active);
            follow.WriteFloat("FollowBeginDistance", FollowBeginDistance);
            follow.WriteFloat("FollowEndDistance", FollowEndDistance);
            follow.WriteVec("Offset", Offset);
            root.Append(follow, "Follow");
        }

        void Follow::Deserialize(Serializer& root) {
            Serializer follow = root.GetChild("Follow");
            follow.ReadBool("Active", Active);
            follow.ReadFloat("FollowBeginDistance", FollowBeginDistance);
            follow.ReadFloat("FollowEndDistance", FollowEndDistance);
            follow.ReadVec("Offset", Offset);
        }
    }
}
