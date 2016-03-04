#ifndef COMPONENT_HEALTH_H
#define COMPONENT_HEALTH_H

#include "Component.h"
#include "Event.h"

namespace ForLeaseEngine {
    namespace Components {
        class Health : public Component {
          public:
            static const ComponentType Type = ComponentType::Health;
            virtual ComponentType GetType() { return Type; }

            float MaxHealth;
            float CurrentHealth;

            Health(Entity& parent, float maxHealth, float health);
            Health(Entity& parent, float maxHealth = 0);
            ~Health();

            void Initialize();
            void Update();
            void Serialize(Serializer& root);
            void Deserialize(Serializer& root);

            void OnDamage(const Event* e);
        };
    }
}

#endif // COMPONENT_HEALTH_H