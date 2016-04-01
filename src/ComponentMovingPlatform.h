/*!
    \file   ComponentMovingPlatform.h
    \author Sean McGeer
    \author Sam Montanari
    \date   3/23/16
    \brief
        Defines the Moving Platform component.
    \see ComponentMovingPlatform.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_MOVING_PLATFORM_H
#define COMPONENT_MOVING_PLATFORM_H

//#include "Keys.h"
#include "KeyboardEvent.h"
#include "Component.h"
#include "Engine.h"
#include "Event.h"
//#include "Entity.h"

namespace ForLeaseEngine
 {
    namespace Components
    {
        class MovingPlatform : public Component
        {
            public:
                enum class Action : char
                {
                    Pause,
                    Away,
                    Back
                };

                enum class Axis : char
                {
                    Horizontal,
                    Vertical
                };

                static const ComponentType Type = ComponentType::MovingPlatform;
                MovingPlatform(Entity & owner, float awaySpeed = 3.0f, float backSpeed = 3.0f, float affectedFieldHeight = 0.25f, float maxDistance = 5.0, float pause = 2.0, Axis direction = Axis::Horizontal);
                ~MovingPlatform();
                virtual ComponentType GetType() { return Type; }

                float AwaySpeed;
                float BackSpeed;
                float MaxMove;
                float PauseTimer;
                Axis Direction;
                float AffectedFieldHeight;

                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                Action CurrentAction;
                Action NextAction;
                float Moved;
                float CurrentPauseTimer;

            private:
                void MoveAway(float dt);
                void MoveBack(float dt);
                void MovePause(float dt);
                Vector LastMovement(bool scaleDt = true); //deprecated
        };
    }
  }

  #endif // COMPONENT_MOVING_PLATFORM_H
