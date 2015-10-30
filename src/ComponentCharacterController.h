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
                static CharacterController* Create(Entity& owner);

                int RightKey;
                int LeftKey;
                int JumpKey;

                float MoveSpeed;
                float JumpSpeed;

                void Initialize();

                void Update();

                void OnKeyDown(const Event* e);
                void OnKeyUp(const Event* e);

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

            private:
                CharacterController(Entity& owner);
        };
    }
}

#endif // CHARACTER_CONTROLLER_H
