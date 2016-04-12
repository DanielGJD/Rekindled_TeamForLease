#ifndef FINALE_ONE_H
#define FINALE_ONE_H

#include "Component.h"
#include <string>
#include "Event.h"
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
            static FinaleOne* Create(Entity& owner);
            void Initialize();
            void Serialize(Serializer& root);
            void Deserialize(Serializer& root);
            void OnCollisionStart(const Event* e);

        private:
            enum Action
            {
                MOVE,
                TRANSITION
            };

            FinaleOne(Entity& owner);
            void Move(float dt);
            void Transition(float dt);
            bool start;
            float targetSize;
            float distance;
            float savePos;
            Point cameraPos;
            Action currentAction;
            bool setup;
            float transitionDelay;
        };
    }
}
#endif // FINALE_ONE_H
