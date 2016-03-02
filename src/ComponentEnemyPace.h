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
                    RIGHT
                };

                static EnemyPace* Create(Entity& owner);
                static const ComponentType Type = ComponentType::EnemyPace;
                EnemyPace(Entity & owner, float speed = 3.0, float maxDistance = 5.0, float pause = 2.0);
                ~EnemyPace();
                virtual ComponentType GetType() { return Type; }

                std::string PaceSound;
                float PaceSpeed;
                float MaxPaceDistance;
                float PauseTimer;
                int Direction;

                void Initialize();
                void Update();
                //void OnPlayerSeen(Event const* e);
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

            private:
                void MoveLeft(float dt);
                void MoveRight(float dt);
                void MovePause(float dt);
                int CurrentAction;
                int NextAction;
                Point& Position;
                float Moved;
                float timer;
        };
    }
  }

  #endif // ENEMY_PACE_H
