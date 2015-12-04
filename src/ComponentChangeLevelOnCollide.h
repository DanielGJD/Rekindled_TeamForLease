/*!
    \file   ComponentChangeLevelOnCollide.h
    \author Christopher Hudson

    \brief
        Defines a component to change the level upon collision with an entity

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_CHANGE_LEVEL_ON_COLLIDE_H
#define COMPONENT_CHANGE_LEVEL_ON_COLLIDE_H

#include "Component.h"
#include "Entity.h"
#include <string>

namespace ForLeaseEngine {
    namespace Components {
        class ChangeLevelOnCollide : public Component {
            public:
                static const ComponentType Type = ComponentType::ChangeLevelOnCollide;
                bool Active;
                std::string LevelName;
                std::string TriggerObjectName;
                float OnSwitchTimeScale;
                float FadeOutTime;

                ChangeLevelOnCollide(Entity& parent, bool active = true, std::string levelName = "", std::string TriggerObjectName = "", float onSwitchTimeScale = 0, float FadeOutTime = 3);
                ~ChangeLevelOnCollide();

                virtual ComponentType GetType();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
            private:
                bool Switching;
                float Timer;
        };
    }
}

#endif // COMPONENT_CHANGE_LEVEL_ON_COLLIDE_H
