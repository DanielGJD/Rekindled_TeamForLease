/*!
    \author Sam Montanari

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

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
            float TransitionTimer;
            float TransitionTimer2;
            float WispLightTimer;


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
            float transitionDelay2;
            float wispLightDelay;
            bool lightSwitch;
        };
    }
}
#endif // FINALE_ONE_H
