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
#include "Keys.h"

namespace ForLeaseEngine {
    namespace Components {
        class ScaleWithKeyboard : public Component{
            public:
                static ScaleWithKeyboard* Create(Entity& owner);
                ScaleWithKeyboard(Entity& owner, bool active = false, float scaleSpeed = 0,
                                  int scaleUpKey = Keys::E, int scaleDownKey = Keys::Q,
                                  float maxXScale = 2, float maxYScale = 2, float scale = 0, std::string ScaleSound = "");
                ~ScaleWithKeyboard();
                virtual ComponentType GetType();
                void Initialize();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void OnKeyDown(Event const* e);
                void OnKeyUp(Event const* e);

                static const ComponentType Type = ComponentType::ScaleWithKeyboard;
                bool Active;
                float ScaleSpeed;
                // Going to remove these
//                int ScaleXUpKey;
//                int ScaleXDownKey;
//                int ScaleYUpKey;
//                int ScaleYDownKey;
                ///////////////////////////////
                int ScaleUpKey;
                int ScaleDownKey;
                // Going to remove these
                float MaxXScale;
                //float MinXScale;
                float MaxYScale;
                float Scale;
                //float MinYScale;
                //////////////////////////////
                //float MinScale;
                //float MaxScale;

                std::string ScaleSound;
            private:
                // Going to remove these
//                bool ScaleXUp;
//                bool ScaleXDown;
//                bool ScaleYUp;
//                bool ScaleYDown;
                ////////////////////////////////////
                bool ScaleUp;
                bool ScaleDown;
                float NaturalWidth;
                float NaturalHeight;
        };
    }
}

#endif // SCALE_WITH_KEYBOARD_H
