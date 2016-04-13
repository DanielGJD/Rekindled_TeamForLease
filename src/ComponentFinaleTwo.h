/*!
    \file   ComponentFinaleTwo.h
    \author Christopher Hudson

    \brief
        Defines a component to control the second finale scene

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef FINALE_TWO_H
#define FINALE_TWO_H

#include "Component.h"
#include <string>

namespace ForLeaseEngine {
    namespace Components {
        class FinaleTwo : public Component {
            public:
                static const ComponentType Type = ComponentType::FinaleTwo;

                std::string DeadForest;
                std::string AliveForestTop;
                std::string AliveForestBottom;
                std::string PulseObject;
                std::string Logo;
                std::string Credits;
                std::string Camera;

                float StartFadeInTime;
                float DelayToFadeOut1;
                float FadeOut1Time;
                float ScreenShakeAmount;
                float DelayToFadeIn1;
                float FadeIn1Time;
                float PulseSpeed;
                float DelayToFadeOut2;
                float FadeOut2Time;
                float DelayToFadeIn2;
                float FadeIn2Time;
                float ScrollSpeed;
                float CreditsDelay;

                std::string RumbleSound;
                std::string Music;

                FinaleTwo(Entity& owner);
                ~FinaleTwo();
                virtual ComponentType GetType();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
            private:
                enum State {
                    START,
                    START_FADE_IN,
                    DELAY_FADE_OUT_1,
                    FADE_OUT_1,
                    DELAY_FADE_IN_1,
                    FADE_IN_1,
                    DELAY_FADE_OUT_2,
                    FADE_OUT_2,
                    DELAY_FADE_IN_2,
                    FADE_IN_2,
                    CREDITS_SCROLL,
                    CREDITS_DELAY
                };

                State CurrentState;
                float StateTimer;
                float PulseTimer;
                Point CameraStartPosition;
        };
    }
}

#endif // FINALE_TWO_H
