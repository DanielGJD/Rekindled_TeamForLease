/*!
    \file   ScaleWithKeyboard.h
    \author Christopher Hudson

    \brief
        Defines a component for scaling an object with the keyboard

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef SCALE_WITH_KEYBOARD_H
#define SCALE_WITH_KEYBOARD_H

#include "Component.h"
#include "Event.h"

namespace ForLeaseEngine {
    namespace Components {
        class ScaleWithKeyboard : public Component{
            public:
                static ScaleWithKeyboard* Create(Entity& owner);
                ~ScaleWithKeyboard();
                virtual ComponentType GetType();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void OnKeyDown(Event const* e);
                void OnKeyUp(Event const* e);

                static const ComponentType Type = ComponentType::ScaleWithKeyboard;
                bool Active;
                float ScaleSpeed;
                int ScaleXUpKey;
                int ScaleXDownKey;
                int ScaleYUpKey;
                int ScaleYDownKey;
                float MaxXScale;
                float MinXScale;
                float MaxYScale;
                float MinYScale;

                std::string ScaleSound;
            private:
                ScaleWithKeyboard(Entity& owner);
                void Initialize();
                bool ScaleXUp;
                bool ScaleXDown;
                bool ScaleYUp;
                bool ScaleYDown;
        };
    }
}

#endif // SCALE_WITH_KEYBOARD_H
