/*!
    \file   ComponentPhysics.cpp
    \author Sean McGeer
    \date   9/24/15
    \brief
        Implements the Physics component.
    \see ComponentPhysics.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentBackgroundMask.h"
#include "ComponentModel.h"
#include "Engine.h"
#include "Interpolation.h"
// #include "Entity.h"

namespace ForLeaseEngine {

    namespace Components {

        BackgroundMask::BackgroundMask(Entity& owner, ModifyingColor mod) : Component(owner, ComponentType::Model) {
            CurrentState = mod;

            switch (CurrentState) {
                case ModifyingColor::Red:
                    Parent.GetComponent<Components::Model>()->ModelColor = Color(0.75f, 0.75f, 1.0f);
                    Parent.GetComponent<Components::Model>()->BlendingMode = MULTIPLY;
                    break;
                case ModifyingColor::Green:
                    Parent.GetComponent<Components::Model>()->ModelColor = Color(1.0f, 0.75f, 0.75f);
                    Parent.GetComponent<Components::Model>()->BlendingMode = MULTIPLY;
                    break;
                case ModifyingColor::Blue:
                    Parent.GetComponent<Components::Model>()->ModelColor = Color(0.75f, 1.0f, 0.75f);
                    Parent.GetComponent<Components::Model>()->BlendingMode = MULTIPLY;
                    break;
            }
        }

        void BackgroundMask::Update() {
            Components::Model* model = Parent.GetComponent<Components::Model>();
            float dt = ForLease->FrameRateController().GetDt() * 0.01f;
            float r = model->ModelColor.GetR();
            float g = model->ModelColor.GetG();
            float b = model->ModelColor.GetB();
            switch (CurrentState) {
                case ModifyingColor::Red:
                    model->ModelColor = Color(r + dt, 0.75f, b - dt);
                    r = model->ModelColor.GetR();
                    if (r >= 1.0f) {
                        model->ModelColor = Color(1.0f, 0.75f, 0.75f);
                        CurrentState = ModifyingColor::Green;
                    }
                    break;
                case ModifyingColor::Green:
                    model->ModelColor = Color(r - dt, g + dt, 0.75f);
                    g = model->ModelColor.GetG();
                    if (g >= 1.0f) {
                        model->ModelColor = Color(0.75f, 1.0f, 0.75f);
                        CurrentState = ModifyingColor::Blue;
                    }
                    break;
                case ModifyingColor::Blue:
                    model->ModelColor = Color(0.75f, g - dt, b + dt);
                    b = model->ModelColor.GetB();
                    if (b >= 1.0f) {
                        model->ModelColor = Color(0.75f, 0.75f, 1.0f);
                        CurrentState = ModifyingColor::Red;
                    }
                    break;
            }
        }

        void BackgroundMask::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned long long int>(Type));
            Serializer bgMask = root.GetChild("BackgroundMask");
            bgMask.WriteUint("Type", static_cast<unsigned long long int>(Type));
            root.Append(bgMask, "BackgroundMask");
        }

        void BackgroundMask::Deserialize(Serializer& root) {
            Serializer bgMask = root.GetChild("BackgroundMask");
        }
    } // Components

} // ForLeaseEngine
