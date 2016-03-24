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
                    Left,
                    Right,
                    Resume
                };

                static MovingPlatform* Create(Entity& owner);
                static const ComponentType Type = ComponentType::MovingPlatform;
                MovingPlatform(Entity & owner, float speed = 3.0, float maxDistance = 5.0, float pause = 2.0, unsigned direction = 0);
                ~MovingPlatform();
                virtual ComponentType GetType() { return Type; }

                std::string PaceSound;
                float PaceSpeed;
                float MaxPaceDistance;
                float PauseTimer;
                float DetectionDelay;
                int Direction;

                void Initialize();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                Vector LastMovement(bool scaleDt = true);

            private:
                void MoveLeft(float dt);
                void MoveRight(float dt);
                void MovePause(float dt);
                Action CurrentAction;
                Action NextAction;
                //Point& Position;
                float Moved;
                float CurrentPauseTimer;
        };
    }
  }

  #endif // COMPONENT_MOVING_PLATFORM_H
