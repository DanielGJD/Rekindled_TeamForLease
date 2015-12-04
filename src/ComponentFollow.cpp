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
                                         unsigned long followEntityID)
                                        : Component(parent, ComponentType::Transform),
                                          Active(active), FollowBeginDistance(followBeginDistance),
                                          FollowEndDistance(followEndDistance), FollowEntityID(followEntityID) {}


        Follow::~Follow() {}

        ComponentType Follow::GetType() { return Type; }

        void Follow::Update() {
            if(!Active)
                return;
            Entity* entity = ForLease->GameStateManager().CurrentState().GetEntityByID(FollowEntityID);
            if(entity) {
                Transform* followTrans = entity->GetComponent<Components::Transform>();
                Transform* myTrans = Parent.GetComponent<Components::Transform>();
                float distance = Point::Distance(followTrans->Position, myTrans->Position);
                float t;
                if(distance < FollowBeginDistance)
                    t = 0;
                else if(distance > FollowEndDistance)
                    t = 1;
                else
                    t = (distance - FollowBeginDistance) / (FollowEndDistance - FollowBeginDistance);

                Vector fullMovement = followTrans->Position - myTrans->Position;
                float moveScale = Interpolation::PowerIn(2, 0, 1, t);
                Vector scaledMovement = Vector::Scale(fullMovement, moveScale * ForLease->FrameRateController().GetDt());
                myTrans->Position += scaledMovement;
            }
        }

        void Follow::Serialize(Serializer& root) {
        }

        void Follow::Deserialize(Serializer& root) {
        }
    }
}
