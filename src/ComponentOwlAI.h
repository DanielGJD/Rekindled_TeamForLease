/*!
    \author Sam Montanari

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_OWL_AI_H
#define COMPONENT_OWL_AI_H

#include "Component.h"
#include "Engine.h"
namespace ForLeaseEngine
{
    namespace Components
    {
        class OwlAI : public Component
        {
            public:
                enum STATE
                {
                    BLINK,
                    LOOK
                };

                static OwlAI* Create(Entity& owner);
                static const ComponentType Type = ComponentType::OwlAI;
                OwlAI(Entity& owner, float blinkTimer = 1.0, float watchTimer = 3.0, Vector dir1 = Vector(0,0), Vector dir2 = Vector(0,0));
                ~OwlAI();
                virtual ComponentType GetType() { return Type; }
                void Initialize();
                void Update();
                void OnObjectSeen(Event const* e);
                void OnObjectNotSeen(Event const* e);
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                float BlinkTime;
                float WatchTime;
                Vector Direction1;
                Vector Direction2;
                std::string Animation;
            private:
                void Look(float dt);
                void Blink(float dt);
                float timer;
                bool pause;
                int currentState;
                bool modelFlip;
                Vector* nextDirection;
        };
    }
}

#endif // COMPONENT_OWL_AI_H
