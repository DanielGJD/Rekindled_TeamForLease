#include "ComponentHealth.h"
#include "DamageEvent.h"
#include "Engine.h"
#include "Entity.h"
#include "State.h"
#include "GameStateManager.h"
#include "LevelComponentCheckpoint.h"
#include <iostream>

namespace ForLeaseEngine {
    namespace Components {
        Health::Health(Entity& parent, float maxHealth, float health)
                      : Component(parent), MaxHealth(maxHealth), CurrentHealth(health) {}

        Health::Health(Entity& parent, float maxHealth)
                      : Component(parent), MaxHealth(maxHealth), CurrentHealth(maxHealth) {}


        Health::~Health() {
            ForLease->Dispatcher.Detach(this, "DamageEvent");
        }

        void Health::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "DamageEvent", &Health::OnDamage, &Parent);
        }

        void Health::Update() {}

        void Health::Serialize(Serializer& root) {
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            Serializer health = root.GetChild("Health");
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            health.WriteFloat("MaxHealth", MaxHealth);
            health.WriteFloat("CurrentHealth", CurrentHealth);
            root.Append(health, "Health");
        }

        void Health::Deserialize(Serializer& root) {
            Serializer health = root.GetChild("Health");
            health.ReadFloat("MaxHealth", MaxHealth);
            health.ReadFloat("CurrentHealth", CurrentHealth);
        }

        void Health::OnDamage(const Event* e) {
            const DamageEvent* damage_e = static_cast<const DamageEvent*>(e);

            if(CurrentHealth > 0 && damage_e->Damage > 0) {
                CurrentHealth -= damage_e->Damage;
                //std::cout << Parent.GetName() << " took " << damage_e->Damage << " damage: " << CurrentHealth << "/" << MaxHealth << std::endl;
            }
            if(CurrentHealth < 0) {
                CurrentHealth = 0;
                LevelComponents::Checkpoint* checkpoints = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Checkpoint>();
                if(checkpoints) {
                    checkpoints->ResetToCheckpoint();
                }
                else {
                    ForLease->GameStateManager().SetAction(Modules::StateAction::Restart);
                }
                //std::cout << Parent.GetName() << " died" << std::endl;

            }
        }
    }
}
