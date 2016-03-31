#include "ComponentDamageOnCollide.h"
#include "CollisionEvent.h"
#include "Engine.h"
#include "DamageEvent.h"

namespace ForLeaseEngine {
    namespace Components {
        DamageOnCollide::DamageOnCollide(Entity& parent, float damage, bool continuous)
                                        : Component(parent, ComponentType::Collision), Damage(damage), Continuous(continuous) {}

        DamageOnCollide::~DamageOnCollide() {
            ForLease->Dispatcher.Detach(this, "CollisionStarted");
            ForLease->Dispatcher.Detach(this, "Collision");
        }

        void DamageOnCollide::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "CollisionStarted", &DamageOnCollide::OnCollisionStarted, &Parent);
            ForLease->Dispatcher.Attach(NULL, this, "Collision", &DamageOnCollide::OnCollision, &Parent);
        }

        void DamageOnCollide::Update() {}

        void DamageOnCollide::Serialize(Serializer& root) {
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));
            Serializer damage = root.GetChild("DamageOnCollide");
            damage.WriteFloat("Damage", Damage);
            damage.WriteBool("Continuous", Continuous);
            root.Append(damage, "DamageOnCollide");
        }

        void DamageOnCollide::Deserialize(Serializer& root) {
            Serializer damage = root.GetChild("DamageOnCollide");
            damage.ReadBool("Continuous", Continuous);
            damage.ReadFloat("Damage", Damage);
        }

        void DamageOnCollide::OnCollisionStarted(const Event* e) {
            if(!Continuous) {
                const CollisionStartedEvent* collision_e = reinterpret_cast<const CollisionStartedEvent*>(e);
                DamageEvent damage_e = DamageEvent(Damage);
                ForLease->Dispatcher.DispatchToParent(&damage_e, collision_e->With);
            }
        }

        void DamageOnCollide::OnCollision(const Event* e) {
            if(Continuous) {
                const CollisionEvent* collision_e = reinterpret_cast<const CollisionEvent*>(e);
                DamageEvent damage_e = DamageEvent(Damage * ForLease->FrameRateController().GetDt());
                ForLease->Dispatcher.DispatchToParent(&damage_e, collision_e->With);
            }
        }
    }
}
