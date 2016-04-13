/*!
    \file   ComponentAutoplay.h
    \author Christopher Hudson

    \brief
        Defines a class for controlling the character by sending arbitrary keyboard events

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_AUTOPLAY_H
#define COMPONENT_AUTOPLAY_H

#include "Component.h"

namespace ForLeaseEngine {
    namespace Components {
        class Autoplay : public Component {
            public:
                static const ComponentType Type = ComponentType::Autoplay;

                bool Active;
                float LeftToggleTime;
                float RightToggleTime;
                float JumpTime;
                float RandomTime;

                Autoplay(Entity& parent, bool active = false,
                         float leftToggleTime = 2, float rightToggleTime = 2,
                         float jumpTime = 2, float randomTime = 1);
                ~Autoplay();
                virtual ComponentType GetType();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
            private:
                float LeftTimer;
                float RightTimer;
                float JumpTimer;

                bool LeftDown;
                bool RightDown;
        };
    }
}

#endif // COMPONENT_AUTOPLAY_H
