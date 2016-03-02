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
                static const ComponentType Type = ComponentType::EnemyPace;
                EnemyPace(Entity & owner);

                ~EnemyPace();
                static EnemyPace* Create(Entity& owner);
                virtual ComponentType GetType()
                {
                    return Type;
                }
                std::string PaceSound;

                float PaceSpeed;
                float PaceDistance;
                float MaxPaceDistance;

                void Initialize();
                void Update();

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

            private:
               bool Detected;
               bool MoveRight;
               bool MoveLeft;

        };


    }



  }

  #endif // ENEMY_PACE_H
