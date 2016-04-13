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
        Health::Health(Entity& parent, float maxHealth, float health, float damageScale, float regenScale)
                      : Component(parent), MaxHealth(maxHealth), CurrentHealth(health), DamageScale(damageScale), RegenScale(regenScale), TookDamage(false) {}

        Health::Health(Entity& parent, float maxHealth)
                      : Health(parent, maxHealth, maxHealth) {}


        Health::~Health() {
            ForLease->Dispatcher.Detach(this, "DamageEvent");
        }

        void Health::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "DamageEvent", &Health::OnDamage, &Parent);
        }

        void Health::Update() {
            if(!TookDamage) {
                CurrentHealth += ForLease->FrameRateController().GetDt() * RegenScale;

                if(CurrentHealth > MaxHealth) {
                    CurrentHealth = MaxHealth;
                }
            }

            Components::Model* model = Parent.GetComponent<Components::Model>();

            if (model) {
                float t = CurrentHealth / MaxHealth;
                model->ModelColor = Color(t, t, t, 1);
            }

            TookDamage = false;
        }

        void Health::Serialize(Serializer& root) {
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            Serializer health = root.GetChild("Health");
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            health.WriteFloat("MaxHealth", MaxHealth);
            health.WriteFloat("CurrentHealth", CurrentHealth);
            health.WriteFloat("DamageScale", DamageScale);
            health.WriteFloat("RegenScale", RegenScale);
            root.Append(health, "Health");
        }

        void Health::Deserialize(Serializer& root) {
            Serializer health = root.GetChild("Health");
            health.ReadFloat("MaxHealth", MaxHealth);
            health.ReadFloat("CurrentHealth", CurrentHealth);
            health.ReadFloat("DamageScale", DamageScale);
            health.ReadFloat("RegenScale", RegenScale);
        }

        void Health::OnDamage(const Event* e) {
            const DamageEvent* damage_e = static_cast<const DamageEvent*>(e);

            if(CurrentHealth > 0 && damage_e->Damage > 0) {
                CurrentHealth -= damage_e->Damage * DamageScale;
                TookDamage = true;
                //std::cout << Parent.GetName() << " took " << damage_e->Damage << " damage: " << CurrentHealth << "/" << MaxHealth << std::endl;
            }
            if(damage_e->Kill) {
                CurrentHealth = 0;
            }

//            Components::Model* model = Parent.GetComponent<Components::Model>();
//            if (model) {
//                float t = CurrentHealth / MaxHealth;
//                model->ModelColor = Color(t, t, t, 1);
//            }

            if(CurrentHealth <= 0) {
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
