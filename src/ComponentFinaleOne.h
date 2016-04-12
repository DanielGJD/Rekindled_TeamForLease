#ifndef FINALE_ONE_H
#define FINALE_ONE_H

#include "Component.h"
#include "State.h"
#include <string>

namespace ForLeaseEngine
{
    namespace Components
    {
        class FinaleOne : public Component
        {
        public:
            static const ComponentType Type = ComponentType::FinaleOne;
            std::string Camera;
            std::string Wisp;
            std::string WispDest;
            std::string CameraDest;
            std::string Light;
            std::string Eyes;
            std::string Trigger;
            float CameraSpeed;
            float CameraSize;
            float CameraGrowth;
            float WispSpeed;
            float PlayerMoveDistance;
            float PlayerMoveSpeed;
            float OverlaySpeed;
            float LightGrowth;


            ~FinaleOne();
            virtual ComponentType GetType();
            void Update();
            static FinaleOne* Create();
            void Initialize();
            void Serialize(Serializer& root);
            void Deserialize(Serializer& root);
            void OnCollisionStart();

        private:
            enum Action
            {
                MOVE,
                TRANSITION
            };

            FinaleOne(Entity& owner);
            void Move();
            void Transition();
            bool start;
            float distance;
            Action currentAction;
            bool setup;
        };
    }
}
#endif // FINALE_ONE_H
