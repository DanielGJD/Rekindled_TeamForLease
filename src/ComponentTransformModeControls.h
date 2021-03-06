/*!
    \file   ComponentTransformModeControls.h
    \author Christopher Hudson

    \brief
        Defines a component for switching between the two game modes

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef TRANSFORM_MODE_CONTROLS_H
#define TRANSFORM_MODE_CONTROLS_H

#include "Component.h"
#include "Event.h"
#include "Keys.h"

namespace ForLeaseEngine {
    namespace Components {
        class TransformModeControls : public Component {
            public:
                TransformModeControls(Entity& owner, float slowMotionSpeed = 0, float normalSpeed = 1,
                                      float influenceRadius = 5, int modeToggleKey = Keys::LeftAlt, std::string transformModeSound = "");
                ~TransformModeControls();
                virtual ComponentType GetType();
                void Initialize();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void Activate();
                void Deactivate();
                void Toggle();

                //void OnKeyDown(Event const* e);
                void OnKeyUp(Event const* e);
                //void OnMouseDown(Event const* e);
                void OnMouseUp(Event const* e);

                static const ComponentType Type = ComponentType::TransformModeControls;
                float SlowMotionSpeed;
                float NormalSpeed;
                float InfluenceRadius;
                int ModeToggleKey;

                std::string TransformModeSound;
            private:
                bool Active;
                bool EntitySelected;
                unsigned long ActiveEntity;
                std::string LastSound;
        };
    }
}

#endif // TRANSFORM_MODE_CONTROLS_H
