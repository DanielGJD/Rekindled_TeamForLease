#include "ComponentSimpleParticleDynamics.h"
#include "ComponentParticleSystem.h"
#include "Engine.h"
#include "Random.h"
#include <list>

namespace ForLeaseEngine {
    namespace Components {
        SimpleParticleDynamics::SimpleParticleDynamics(Entity& parent, bool active,
                                                       const Vector& force, const Vector& forceRandom, float torque, float torqueRandom,
                                                       float growth, float growthRandom, float drag, float dragRandom)
                                                      : Component(parent, ComponentType::ParticleSystem),
                                                        Active(active), Force(force), ForceRandom(forceRandom), Torque(torque), TorqueRandom(torqueRandom),
                                                        Growth(growth), GrowthRandom(growthRandom), Drag(drag), DragRandom(dragRandom) {}

        SimpleParticleDynamics::~SimpleParticleDynamics() {}

        ComponentType SimpleParticleDynamics::GetType() { return Type; }

        void SimpleParticleDynamics::Update() {
            if(Active) {
                ParticleSystem* system = Parent.GetComponent<ParticleSystem>();
                std::list<Particle*> const* particles = system->GetActiveParticles();

                if(!particles->empty()) {
                    double dt = ForLease->FrameRateController().GetDt();

                    for(std::list<Particle*>::const_iterator i = particles->begin(); i != particles->end(); ++i) {
                        (*i)->Velocity[0] += (Force[0] + RandomFloat(-ForceRandom[0], ForceRandom[0]) - (*i)->Velocity[0] * Drag - (*i)->Velocity[0] * RandomFloat(-DragRandom, DragRandom)) * dt;
                        (*i)->Velocity[1] += (Force[1] + RandomFloat(-ForceRandom[1], ForceRandom[1]) - (*i)->Velocity[1] * Drag - (*i)->Velocity[1] * RandomFloat(-DragRandom, DragRandom)) * dt;
                        (*i)->RotationalVelocity += (Torque - (*i)->RotationalVelocity * Drag - (*i)->RotationalVelocity * RandomFloat(-DragRandom, DragRandom)) * dt;
                        (*i)->Size += (Growth + RandomFloat(-GrowthRandom, GrowthRandom)) * dt;
                        if((*i)->Size < 0) {
                            (*i)->Size = 0;
                        }
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
