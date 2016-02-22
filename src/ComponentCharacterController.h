/*!
    \file   ComponentCharacterController.h
    \author Christopher Hudson

    \brief
        Defines a component for controlling the main character

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H

#include "Keys.h"
#include "KeyboardEvent.h"
#include "Component.h"
#include "Engine.h"

namespace ForLeaseEngine {
    namespace Components {
        class CharacterController : public Component {
            public:
                static const ComponentType Type = ComponentType::PlayerController;
                virtual ComponentType GetType() { return Type; }
                static CharacterController* Create(Entity& owner);
                CharacterController(Entity& owner);
                ~CharacterController();

                double dt = ForLease->FrameRateController().GetDt();

                int RightKey;
                int LeftKey;
                int JumpKey;

                float MoveSpeed;
                float JumpSpeed;

                float Drag;
                std::string WalkSound;
                std::string JumpSound;
                std::string LandSound;

                std::string WalkAnimation;
                std::string JumpAnimation;

                void Initialize();

                void Update();

                void OnKeyDown(const Event* e);
                void OnKeyUp(const Event* e);

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

            private:
                bool CanJump;
                bool CheckMove;
                unsigned int LastAnimationFrame;
        };
    }
}

#endif // CHARACTER_CONTROLLER_H
