#ifndef ENEMY_PACE_H
#define ENEMY_PACE_H

#include "Keys.h"
#include "KeyboardEvent.h"
#include "Component.h"
#include "Engine.h"

namespace ForLeaseEngine
 {
    namespace Components
    {
        class EnemyPace : public Component
        {
            public:
                //static const ComponentType Type = ComponentType::PaceChecker;
                virtual ComponentType GetType()
                {
                    return Type;
                }
                EnemyPace * Create(Entity& owner);
                EnemyPace(Entity & owner);
                ~EnemyPace();
                std::string PaceSound;

                void Initialize();
                void Update();

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

            private:
               bool Detected;
               bool MoveRight;
               bool MoveLeft;
               float PaceSpeed;
               float PaceDistance;
               float MaxPaceDistance;
        };


    }



  }

  #endif // ENEMY_PACE_H
