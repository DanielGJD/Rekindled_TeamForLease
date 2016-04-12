/*!
    \file   ComponentFollow.h
    \author Christopher Hudson

    \brief
        Defines a component to make an entity follow another entity

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_FOLLOW
#define COMPONENT_FOLLOW

#include "Component.h"
#include "Entity.h"

namespace ForLeaseEngine {
    namespace Components {
        class Follow : public Component {
            public:
                static const ComponentType Type = ComponentType::Follow;
                bool Active;
                float FollowBeginDistance;
                float FollowEndDistance;
                unsigned long FollowEntityID;
                Vector Offset;
                float Speed;

                Follow(Entity& parent, bool active = true, float followBeginDistance = 0, float followEndDistance = 1, unsigned long followEntityID = 0, Vector const& offset = Vector(), float speed = 10);
                ~Follow();

                virtual ComponentType GetType();

                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
            private:
                Vector AlteredOffset;
        };
    }
}

#endif // COMPONENT_FOLLOW
