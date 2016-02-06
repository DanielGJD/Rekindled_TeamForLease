#include "ComponentSimpleParticleDynamics.h"
#include "ComponentParticleSystem.h"
#include "Engine.h"
#include <list>

namespace ForLeaseEngine {
    namespace Components {
        SimpleParticleDynamics::SimpleParticleDynamics(Entity& parent, bool active,
                                                       Vector force, float torque, float growth, float drag)
                                                      : Component(parent, ComponentType::ParticleSystem),
                                                        Active(active), Force(force), Torque(torque), Growth(growth), Drag(drag) {}

        SimpleParticleDynamics::~SimpleParticleDynamics() {}

        ComponentType SimpleParticleDynamics::GetType() { return Type; }

        void SimpleParticleDynamics::Update() {
            if(Active) {
                ParticleSystem* system = Parent.GetComponent<ParticleSystem>();
                std::list<Particle*> const* particles = system->GetActiveParticles();

                if(!particles->empty()) {
                    double dt = ForLease->FrameRateController().GetDt();

                    for(std::list<Particle*>::const_iterator i = particles->begin(); i != particles->end(); ++i) {
                        (*i)->Velocity[0] += (Force[0] - (*i)->Velocity[0] * Drag) * dt;
                        (*i)->Velocity[1] += (Force[1] - (*i)->Velocity[1] * Drag) * dt;
                        (*i)->RotationalVelocity += Torque * Drag * dt;
                        (*i)->Size += Growth * dt;
                    }
                }
            }
        }

        void SimpleParticleDynamics::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned int>(Type));
            Serializer dynamics = root.GetChild("SimpleParticleDynamics");
            dynamics.WriteUint("Type", static_cast<unsigned int>(Type));
            dynamics.WriteBool("Active", Active);
            dynamics.WriteVec("Force", Force);
            dynamics.WriteFloat("Torque", Torque);
            dynamics.WriteFloat("Growth", Growth);
            dynamics.WriteFloat("Drag", Drag);
            root.Append(dynamics, "SimpleParticleDynamics");
        }

        void SimpleParticleDynamics::Deserialize(Serializer& root) {
            Serializer dynamics = root.GetChild("SimpleParticleDynamics");
            dynamics.ReadBool("Active", Active);
            dynamics.ReadVec("Force", Force);
            dynamics.ReadFloat("Torque", Torque);
            dynamics.ReadFloat("Growth", Growth);
            dynamics.ReadFloat("Drag", Drag);
        }
    }
}
