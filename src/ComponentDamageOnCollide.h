#ifndef DAMAGE_ON_COLLIDE_H
#define DAMAGE_ON_COLLIDE_H

#include "Component.h"
#include "Event.h"

namespace ForLeaseEngine {
    namespace Components {
        class DamageOnCollide : public Component {
            public:
                static const ComponentType Type = ComponentType::DamageOnCollide;
                virtual ComponentType GetType() { return Type; }

                float Damage;
                bool Continuous;

                DamageOnCollide(Entity& parent, float damage = 10, bool continuous = false);
                ~DamageOnCollide();

                void Initialize();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void OnCollisionStarted(const Event* e);
                void OnCollision(const Event* e);
        };
    }
}

#endif // DAMAGE_ON_COLLIDE_H
