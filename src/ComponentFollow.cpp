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
#include "LevelComponentRenderer.h"

namespace ForLeaseEngine {
    namespace Components {
        Follow::Follow(Entity& parent, bool active,
                                         float followBeginDistance, float followEndDistance,
                                         unsigned long followEntityID, Vector const& offset, float speed)
                                        : Component(parent, ComponentType::Transform),
                                          Active(active), FollowBeginDistance(followBeginDistance),
                                          FollowEndDistance(followEndDistance), FollowEntityID(followEntityID), Offset(offset), Speed(speed) {}


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
                double distance = direction.Magnitude();
                direction.Normalize();

//                if(distance < FollowBeginDistance) {
//                    std::cout << "Closer than min distance" << std::endl;
//                    return;
//                }
//                else if(distance > FollowEndDistance) {
//                    std::cout << "Beyond max distance, place at max" << std::endl;
//                    myTrans->Position += direction * (distance - FollowEndDistance);
//                }
                if(distance > FollowBeginDistance) {
                    float delta = FollowEndDistance - FollowBeginDistance;
                    if(delta < 0.00001 && delta > -0.00001) {
                        myTrans->Position += direction * (distance - FollowEndDistance);
                    }
                    else {
                        float t = (distance - FollowBeginDistance) / (FollowEndDistance - FollowBeginDistance);
                        myTrans->Position += direction * Interpolation::Linear(0, Speed, t) * ForLease->FrameRateController().GetDt();
                    }
                }
                //myTrans->Position += direction * ForLease->FrameRateController().GetDt();
            }
        }

        void Follow::Serialize(Serializer& root) {
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            Serializer follow = root.GetChild("Follow");
            follow.WriteBool("Active", Active);
            follow.WriteFloat("FollowBeginDistance", FollowBeginDistance);
            follow.WriteFloat("FollowEndDistance", FollowEndDistance);
            follow.WriteUint("FollowEntityID", FollowEntityID);
            follow.WriteVec("Offset", Offset);
            root.Append(follow, "Follow");
        }

        void Follow::Deserialize(Serializer& root) {
            unsigned id;
            Serializer follow = root.GetChild("Follow");
            follow.ReadBool("Active", Active);
            follow.ReadFloat("FollowBeginDistance", FollowBeginDistance);
            follow.ReadUint("FollowEntityID", id);
            FollowEntityID = id;
            follow.ReadFloat("FollowEndDistance", FollowEndDistance);
            follow.ReadVec("Offset", Offset);
        }
    }
}
