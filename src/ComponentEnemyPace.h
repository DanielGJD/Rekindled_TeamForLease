/*!
    \author Sam Montanari

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_ENEMY_PACE_H
#define COMPONENT_ENEMY_PACE_H

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
        class EnemyPace : public Component
        {
            public:
                enum Action
                {
                    PAUSE,
                    LEFT,
                    RIGHT,
                    WATCH,
                    RESUME
                };

                static EnemyPace* Create(Entity& owner);
                static const ComponentType Type = ComponentType::EnemyPace;
                EnemyPace(Entity & owner, float speed = 3.0, float maxDistance = 5.0, float pause = 2.0, unsigned direction = 0);
                ~EnemyPace();
                virtual ComponentType GetType() { return Type; }

                std::string Animation;
                float PaceSpeed;
                float MaxPaceDistance;
                float PauseTimer;
                float DetectionDelay;
                float ResumeTime;
                int Direction;

                void Initialize();
                void Update();
                void OnPlayerSeen(Event const* e);
                void OnPlayerNotSeen(Event const* e);
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

            private:
                bool playerDetected;
                void MoveLeft(float dt);
                void MoveRight(float dt);
                void MovePause(float dt);
                void WatchPlayer(float dt);
                int CurrentAction;
                int NextAction;
                Point& Position;
                float Moved;
                float ptimer;
                float dtimer;
                float rtimer;
        };
    }
  }

  #endif // ENEMY_PACE_H
